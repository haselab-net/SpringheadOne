 /*  FILE: vt_glove.c  --                                        */
00002 /*                                                              */
00003 /*  CyberGlove initialization and control routines              */
00004 /*                                                              */
00005 /*  ==========================================================  */
00006 /*  --  Copyright (C) 1990,91,92,93 Virtual Technologies    --  */
00007 /*  --                                                      --  */
00008 /*  --    Original author: William L. Chapin                --  */
00009 /*  --    Heavily modified by: Larry Edwards                --  */
00010 /*  ==========================================================  */
00011 
00012 
00013 #include <gadget/Devices/DriverConfig.h>
00014 #include <stdio.h>
00015 #include <stdlib.h>
00016 #include <string.h>
00017 #include <stdarg.h>
00018 #include <sys/types.h>
00019 #include <unistd.h>
00020 #include <termios.h>
00021 #include <math.h>
00022 
00023 #include <gadget/Devices/VirtualTechnologies/CyberGlove/CyberGlove.h>
00024 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_types.h>
00025 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_globals.h>
00026 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_glove-types.h>
00027 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_error.h>
00028 
00029 #define delay(t) (usleep(t*100)) /* t is in milliseconds */
00030 
00031 
00032 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00033 /*                                                             */
00034 /*  Private Global Constants                                   */
00035 /*                                                             */
00036 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00037 
00038 
00039 /* some handy delays */
00040 static const int STATUS_WAIT = 1000, REBOOT_WAIT = 2500;
00041 
00042 
00043 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00044 /*                                                             */
00045 /*  Forward Declarations for Private Functions                 */
00046 /*                                                             */
00047 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00048 
00049 void abduct_18(void);
00050 
00051 
00052 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00053 /*                                                             */
00054 /*  Initialization                                             */
00055 /*                                                             */
00056 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00057 
00058 
00059 CbGlove
00060 CyberGloveBasic::allocate_CbGlove(void)
00061 {
00062   CbGlove newglove;
00063   CbGlovePrivateStruct *private_data;
00064   int finger, joint;
00065 
00066   if ((newglove = (CbGlove) calloc(1,sizeof(CbGloveStruct))) == NULL)
00067     vt_fatal_unix_error("allocate_CbGlove","calloc");
00068 
00069   if ((newglove->private_data =
00070        (CbGlovePrivateStruct *) calloc(1,sizeof(CbGlovePrivateStruct))) == NULL)
00071     vt_fatal_unix_error("allocate_CbGlove","calloc");
00072 
00073   newglove->portfd = -1;
00074   newglove->timestamp = 0L;
00075 
00076   private_data = newglove->private_data;
00077 
00078   private_data->param_flags.word = CG_STARTUP_FLAGS; /*  Powerup defaults   */
00079 
00080   for (finger = THUMB; finger <= FINGERS; finger++)
00081     for (joint = MCP; joint < JOINTS; joint++)
00082     {
00083       private_data->mapping[finger][joint].offset = 50;
00084       private_data->mapping[finger][joint].gain =  0.4;
00085     }
00086 
00087 //  private_data->abduct_fingers = abduct_18;
00088 
00089   memset(private_data->ascii_record,0,CG_RECSIZE);
00090 
00091   return (newglove);
00092 }
00093 
00094 int
00095 CyberGloveBasic::glove_boot(CbGlove glove)
00096 {
00097   CbGlovePrivate private_data = glove->private_data;
00098 
00099   /* reboot the CbGlove Interface Unit */
00100   if (reboot_CGIU(glove,NULL) < Ok)
00101     return(vt_print_error("glove_boot"));
00102 
00103   /* check to see if CbGlove is connected to the interface unit and valid */
00104   if (glove_ok_query(glove,NULL) < Ok)
00105     return(vt_print_error("glove_boot"));
00106 
00107   if (!(private_data->param_flags.word & CG_GLOVE_VALID_FLAG))
00108     return(vt_set_error("glove_boot",CG_ERROR2));
00109 
00110   if (!(private_data->param_flags.word & CG_GLOVE_IN_FLAG))
00111     return (vt_set_error("glove_boot",CG_ERROR3));
00112 
00113   /* read the parameter flags */
00114   if (read_param_flags(glove,NULL) < Ok)
00115     return (vt_print_error("glove_boot"));
00116 
00117   /* find out which sensors we can enable */
00118   if (get_avail_sensor_mask(glove,NULL) < Ok)
00119     return (vt_print_error("glove_boot"));
00120   private_data->sensors_in_use.word = private_data->available_sensors.word;
00121 
00122   /* find out how many sensors we got */
00123   if (get_max_num_sensors(glove,NULL) < Ok)
00124     return (vt_print_error("glove_boot"));
00125   private_data->num_values_returned = private_data->num_enabled_sensors =
00126     private_data->num_sensors_available;
00127 
00128 /*
00129   switch (private_data->num_sensors_available)
00130   {
00131   case 18:
00132     private_data->abduct_fingers = abduct_18; break;
00133   default:
00134     private_data->abduct_fingers = abduct_18; break;
00135   }
00136 */
00137 
00138   return (Ok);
00139 }
00140 
00141 int
00142 CyberGloveBasic::vt_open_glove_port(char *dev_name, int baudrate)
00143 {
00144   if ((current_glove->portfd = vt_serial_open(dev_name,baudrate)) < Ok)
00145     return (vt_print_error("vt_open_glove_port"));
00146 
00147   if (glove_boot(current_glove) < Ok)
00148   {
00149     vt_serial_close(current_glove->portfd);
00150     current_glove->portfd = -1;
00151     return (vt_print_error("vt_open_glove_port"));
00152   }
00153 
00154   return (Ok);
00155 }
00156 
00157 void
00158 CyberGloveBasic::vt_close_glove_port(CbGlove glove)
00159 {
00160   vt_serial_close(glove->portfd);
00161   glove->portfd = -1;
00162 }
00163 
00164 CbGlove
00165 CyberGloveBasic::vt_create_CbGlove(Boolean connect, ...)
00166 {
00167   char *dev_name;
00168   int baudrate;
00169   va_list address_arg1;
00170 
00171   va_start(address_arg1,connect);
00172   
00173   current_glove = allocate_CbGlove();
00174   current_glove_private = current_glove->private_data;
00175 
00176   if (connect)
00177   {
00178     dev_name = va_arg(address_arg1, char *);
00179     baudrate = va_arg(address_arg1,int);
00180     if (vt_open_glove_port(dev_name,baudrate) < Ok)
00181       vt_print_error("vt_create_CbGlove");
00182   }
00183   va_end(address_arg1);
00184 
00185   return (current_glove);
00186 }
00187 
00188 void
00189 CyberGloveBasic::vt_destroy_CbGlove(CbGlove glove)
00190 {
00191   if (glove->portfd != -1)
00192     vt_serial_close(glove->portfd);
00193   free(glove->private_data);
00194   free(glove);
00195 }
00196 
00197 
00198 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00199 /*  VT_UNPROCESS_GLOVE_ANGLES --                                */
00200 /*                                                              */
00201 /*  set the raw glove values from the joint angles ... only     */
00202 /*  used to initialize raw values so that when glove is off     */
00203 /*  the joint_angles will retain reasonable values.             */
00204 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00205 
00206 void
00207 CyberGloveBasic::vt_unprocess_glove_angles(void)
00208 {
00209   int finger, joint;
00210   glove_sensor_map_struct (*mapping)[MAX_GROUP_VALUES];
00211   volatile float (*angle)[MAX_GROUP_VALUES];
00212   unsigned char (*raw_value)[MAX_GROUP_VALUES];
00213 
00214   mapping = current_glove_private->mapping;
00215   angle = current_glove->joint_angle;
00216   raw_value = current_glove->raw_value;
00217 
00218   /* NOTE: we negate gain here, so that sense of rotations is appropriate */
00219   /* for the digit coordinate systems                                     */
00220   for (finger = THUMB; finger < FINGERS+1; finger++)
00221   {
00222     for (joint = MCP; joint < JOINTS; joint++)
00223     {
00224        // XXX: This has been modified to deal with trunc() being undefined
00225        // on Linux.  This should be reviewed in closer detail during release
00226        // testing at the very least.
00227        // -PH January 4, 2002.
00228        double joint_val =   mapping[finger][joint].offset
00229                           - mapping[finger][joint].gain 
00230                           * angle[finger][joint];
00231 
00232        double joint_val_trunc = (joint_val < 0) ? ceil(joint_val)
00233                                                 : floor(joint_val);
00234        raw_value[finger][joint] = static_cast<unsigned char>(joint_val_trunc);
00235 
00236        // XXX: some systems have a faulty trunc (undefined)...
00237 /*
00238       raw_value[finger][joint] = static_cast<unsigned char>(trunc(mapping[finger][joint].offset -
00239                             mapping[finger][joint].gain *
00240                angle[finger][joint]));
00241 */
00242     }
00243   }
00244 
00245   /* if we have a left handed glove negate abduction angles */
00246   if (!(current_glove_private->param_flags.word & CG_RIGHT_HAND_FLAG))
00247   {
00248     for (finger = THUMB; finger < FINGERS; finger++)
00249       raw_value[finger][ABDUCT] = -raw_value[finger][ABDUCT];
00250 
00251     raw_value[CARPUS][WRIST_YAW] = -raw_value[CARPUS][WRIST_YAW];
00252   }
00253 }
00254 
00255 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00256 /*                                                             */
00257 /*  routines to check the glove's responses                    */
00258 /*                                                             */
00259 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00260 
00261 
00262 int
00263 CyberGloveBasic::query_reply_head_check(int portfd, char *query_string)
00264 {
00265   unsigned char reply[2];
00266 
00267   if (vt_serial_read_bytes(portfd,reply,2) < Ok)
00268     return (vt_print_error("query_reply_head_check"));
00269 
00270   if (strncmp(query_string,(const char *)reply,2) != 0)
00271     return (vt_set_error("query_reply_head_check",CG_ERROR5));
00272 
00273   return (Ok);
00274 }
00275 
00276 int
00277 CyberGloveBasic::reply_tail_check(CbGlove glove)
00278 {
00279   int i, lastbytes[5], portfd = glove->portfd;
00280   CbGlovePrivate private_data = glove->private_data;
00281   Boolean sample_error = FALSE, glove_error = FALSE;
00282 
00283   if ((lastbytes[0] = vt_serial_read_byte(portfd)) == -1)
00284     return (vt_print_error("reply_tail_check"));
00285 
00286   if (lastbytes[0] == CG_TERMINATOR)
00287     return (Ok);
00288 
00289   for (i=1; i < 5; i++)
00290   {
00291     if ((lastbytes[i] = vt_serial_read_byte(portfd)) == -1)
00292       return (vt_print_error("reply_tail_check"));
00293 
00294     if (i & 1)       /* i.e. if i is odd */
00295     {
00296       switch (lastbytes[i])
00297       {
00298       case 'e':   break;      /* we dropped a byte somewhere */
00299       case 's':
00300    sample_error = TRUE;
00301    break;
00302       case 'g':
00303    glove_error = TRUE;
00304    break;
00305       case CG_TERMINATOR:  /* should only find terminator on even i */
00306    return (vt_set_error("reply_tail_check",CG_ERROR5));
00307       default:
00308    vt_serial_clear_to_terminator(portfd,CG_TERMINATOR);
00309    return (vt_set_error("reply_tail_check",CG_ERROR5));
00310       }
00311     }
00312     else
00313       switch (lastbytes[i])
00314       {
00315       case 'e':   break;
00316       case 's':break;      /* we dropped a byte somewhere */
00317       case 'g':break;      /* we dropped a byte somewhere */
00318       case CG_TERMINATOR:
00319    if (sample_error && glove_error)
00320    {
00321      private_data->param_flags.word &= ~CG_GLOVE_IN_FLAG;
00322      return (vt_set_error("reply_tail_check",CG_ERROR9));
00323    }
00324    else if (glove_error)
00325    {
00326      private_data->param_flags.word &= ~CG_GLOVE_IN_FLAG;
00327      return (vt_set_error("reply_tail_check",CG_ERROR3));
00328    }
00329    else
00330      return (vt_set_error("reply_tail_check",CG_ERROR10));
00331       default:
00332    vt_serial_clear_to_terminator(portfd,CG_TERMINATOR);
00333    return (vt_set_error("reply_tail_check",CG_ERROR5));
00334       }
00335   }
00336 
00337   printf("\nYO BABY!\n"); fflush(stdout);
00338   return Ok;
00339 }
00340 
00341 int
00342 CyberGloveBasic::command_reply_check(CbGlove glove, char command_char)
00343 {
00344   unsigned char reply[4] = "FFF";
00345 
00346   if (vt_serial_read_bytes(glove->portfd,reply,2) < Ok)
00347     return (vt_print_error("command_reply_check"));
00348 
00349   if (reply_tail_check(glove) < Ok)
00350     return (vt_print_error("command_reply_check"));
00351 
00352   return (Ok);
00353 }
00354 
00355 
00356 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00357 /*                                                             */
00358 /*  routines to read data returned by the glove                */
00359 /*                                                             */
00360 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00361 
00362 
00363 int
00364 CyberGloveBasic::read_and_decode_timestamp(int portfd, unsigned long *timestamp)
00365 {
00366   word_byte_union newstamp;
00367   int bitflipflags;
00368 
00369   if ((bitflipflags = vt_serial_read_byte(portfd)) == -1)
00370     return (vt_print_error("read_and_decode_timestamp"));
00371 
00372   if (bitflipflags == 0)
00373     return (vt_set_error("read_and_decode_timestamp",CG_ERROR8));
00374 
00375   if (vt_serial_read_longs(portfd,&newstamp.word,1) < Ok)
00376     return (vt_print_error("read_and_decode_timestamp"));
00377 
00378   if ((newstamp.byte[LONG_BYTE_0] == 0) || (newstamp.byte[LONG_BYTE_1] == 0) ||
00379       (newstamp.byte[LONG_BYTE_2] == 0) || (newstamp.byte[LONG_BYTE_3] == 0))
00380     return (vt_set_error("read_and_decode_timestamp",CG_ERROR8));
00381 
00382   newstamp.byte[LONG_BYTE_0] = (bitflipflags & 0x01) ^
00383                                 newstamp.byte[LONG_BYTE_0];
00384   bitflipflags = ((unsigned char) bitflipflags) >> 1;
00385   newstamp.byte[LONG_BYTE_1] = (bitflipflags & 0x01) ^
00386                                 newstamp.byte[LONG_BYTE_1];
00387   bitflipflags = ((unsigned char) bitflipflags) >> 1;
00388   newstamp.byte[LONG_BYTE_2] = (bitflipflags & 0x01) ^
00389                                 newstamp.byte[LONG_BYTE_2];
00390   bitflipflags = ((unsigned char) bitflipflags) >> 1;
00391   newstamp.byte[LONG_BYTE_3] = (bitflipflags & 0x01) ^
00392                                 newstamp.byte[LONG_BYTE_3];
00393 
00394   *timestamp = newstamp.word;
00395 
00396   return(Ok);
00397 }
00398 
00399 int
00400 CyberGloveBasic::vt_read_glove_data(void)
00401 {
00402   int portfd = current_glove->portfd;
00403   unsigned long sensor_mask, stencil = 0x1;
00404   unsigned char value[MAX_SENSOR_VALUES];
00405   int8_t status_byte;
00406   int reply_head,i,joint,finger,num_values, count = 0;
00407   char message[256];
00408 
00409   sensor_mask = current_glove_private->sensors_in_use.word;
00410   num_values = current_glove_private->num_values_returned;
00411   if (num_values == 0)
00412     return (Ok);
00413 
00414   if ((reply_head = vt_serial_read_byte(current_glove->portfd)) == -1)
00415     return (vt_print_error("vt_read_glove_data"));
00416 
00417   if ((reply_head != CG_REQUEST_DATA_CH) && (reply_head != CG_SEND_STREAM_CH))
00418   {
00419     vt_serial_clear_to_terminator(portfd,CG_TERMINATOR);
00420     sprintf(message,"vt_read_glove_data: head byte = %d:",reply_head);
00421     return (vt_set_error(message,CG_ERROR11));
00422   }
00423 
00424   if (vt_serial_read_bytes(portfd,value,num_values) < Ok)
00425     return (vt_print_error("vt_read_glove_data"));
00426 
00427   /* sensor values should never be zero ... check them */
00428   for (i=0; i < num_values; i++)
00429     if (value[i] == 0)
00430       return (vt_set_error("vt_read_glove_data",CG_ERROR8));
00431 
00432   sensor_mask = current_glove_private->sensors_in_use.word;
00433   for (finger=0; finger < MAX_SENSOR_GROUPS; finger++)
00434     for (joint=0; joint < MAX_GROUP_VALUES; joint++)
00435     {
00436       if (sensor_mask & stencil)
00437    current_glove->raw_value[finger][joint] = value[count++];
00438       stencil <<= 1;
00439     }
00440 
00441   if (current_glove_private->param_flags.word & CG_TIME_STAMP_FLAG)
00442     if (read_and_decode_timestamp(portfd,&current_glove->timestamp) < Ok)
00443       return (vt_print_error("vt_read_glove_data"));
00444 
00445   if (current_glove_private->param_flags.word & CG_STATUS_BYTE_FLAG)
00446   {
00447     if ((status_byte = vt_serial_read_byte(portfd)) == -1)
00448       return (vt_print_error("vt_read_glove_data"));
00449     else
00450       current_glove_private->param_flags.byte[LONG_BYTE_0] = status_byte;
00451   }
00452 
00453   if (reply_tail_check(current_glove) < Ok)
00454     return (vt_print_error("vt_read_glove_data"));
00455 
00456   return (Ok);
00457 }
00458 
00459 int
00460 CyberGloveBasic::vt_read_processed_glove_data(void)
00461 {
00462   if (vt_read_glove_data() < Ok)
00463     return (vt_print_error("vt_read_processed_glove_data"));
00464 
00465   vt_process_glove_data();
00466 
00467   return (Ok);
00468 }
00469 
00470 
00471 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00472 /*                                                             */
00473 /*  routines to send glove commands and read glove data        */
00474 /*                                                             */
00475 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00476 
00477 
00478 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00479 /* SET_BAUD_RATE --                                            */
00480 /*                                                             */
00481 /* Args:                                                       */
00482 /*   CbGlove glove                                          */
00483 /*   unsigned char baudbyte                                    */
00484 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00485 
00486 int
00487 CyberGloveBasic::set_baud_rate(CbGlove glove, va_list temp)
00488 {
00489   int portfd = glove->portfd;
00490   CbGlovePrivate private_data = glove->private_data;
00491   unsigned char baudbyte, dontcaremask = 0x37;
00492 
00493   baudbyte = va_arg(temp, int);
00494   baudbyte &= dontcaremask;
00495 
00496   /* send the set baud rate command */
00497   if ((vt_serial_write_byte(portfd,CG_BAUD_RATE_CH) < Ok) ||
00498       (vt_serial_write_byte(portfd,baudbyte) < Ok))
00499     return(vt_print_error("set_baud_rate"));
00500 
00501   /* check the response */
00502   if (command_reply_check(glove,CG_BAUD_RATE_CH) < Ok)
00503     return(vt_print_error("set_baud_rate"));
00504 
00505   private_data->baud_divide_byte = baudbyte;
00506 
00507   return (Ok);
00508 }
00509 
00510 
00511 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00512 /* ENABLE_TIMESTAMP --                                         */
00513 /*                                                             */
00514 /*                                                             */
00515 /* Args:                                                       */
00516 /*   CbGlove glove                                          */
00517 /*   Boolean enabled                                           */
00518 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00519 
00520 int
00521 CyberGloveBasic::enable_timestamp(CbGlove glove, va_list temp)
00522 {
00523   int portfd = glove->portfd;
00524   CbGlovePrivate private_data = glove->private_data;
00525   int enabled;
00526 
00527   enabled = va_arg(temp, int);
00528 
00529   /* send the timestamp enable/disable command */
00530   if ((vt_serial_write_byte(portfd,CG_TIME_STAMP_CH) < Ok) ||
00531       (vt_serial_write_byte(portfd,enabled) < Ok))
00532     return(vt_print_error("enable_timestamp"));
00533 
00534   /* check the response */
00535   if (command_reply_check(glove,CG_TIME_STAMP_CH) < Ok)
00536     return(vt_print_error("enable_timestamp"));
00537 
00538   private_data->param_flags.word = (enabled) ?
00539                             private_data->param_flags.word | CG_TIME_STAMP_FLAG :
00540                             private_data->param_flags.word & ~CG_TIME_STAMP_FLAG;
00541 
00542   return (Ok);
00543 }
00544 
00545 
00546 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00547 /* ENABLE_FILTER --                                            */
00548 /*                                                             */
00549 /*                                                             */
00550 /* Args:                                                       */
00551 /*   CbGlove glove                                          */
00552 /*   Boolean enabled                                           */
00553 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00554 
00555 int
00556 CyberGloveBasic::enable_filter(CbGlove glove, va_list temp)
00557 {
00558   int portfd = glove->portfd;
00559   CbGlovePrivate private_data = glove->private_data;
00560   Boolean enabled;
00561 
00562   enabled = va_arg(temp, int);
00563 
00564   /* send the filter enable/disable command */
00565   if ((vt_serial_write_byte(portfd,CG_FILTER_CH) < Ok) ||
00566       (vt_serial_write_byte(portfd,enabled) < Ok))
00567     return(vt_print_error("enable_filter"));
00568 
00569   /* check the response */
00570   if (command_reply_check(glove,CG_FILTER_CH) < Ok)
00571     return(vt_print_error("enable_filter"));
00572 
00573   private_data->param_flags.word = (enabled ?
00574              private_data->param_flags.word | CG_FILTER_FLAG :
00575              private_data->param_flags.word & ~CG_FILTER_FLAG);
00576   return (Ok);
00577 }
00578 
00579 
00580 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00581 /* REQUEST_SINGLE_SAMPLE --                                    */
00582 /*                                                             */
00583 /*                                                             */
00584 /* Args:                                                       */
00585 /*   CbGlove glove                                          */
00586 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00587 
00588 int
00589 CyberGloveBasic::request_single_sample(CbGlove glove, va_list temp)
00590 {
00591   int portfd = glove->portfd;
00592 
00593   /* send the request sensor value command */
00594   if (vt_serial_write_byte(portfd,CG_REQUEST_DATA_CH) < Ok)
00595     return(vt_print_error("request_single_sample"));
00596 
00597   /* we don't wait for the response so we can do other things while */
00598   /* CGIU is sending data */
00599 
00600   return (Ok);
00601 }
00602 
00603 
00604 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00605 /* REQUEST_SAMPLE_STREAM --                                    */
00606 /*                                                             */
00607 /*                                                             */
00608 /* Args:                                                       */
00609 /*   CbGlove glove                                          */
00610 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00611 
00612 int
00613 CyberGloveBasic::request_sample_stream(CbGlove glove, va_list temp)
00614 {
00615   int portfd = glove->portfd;
00616 
00617   /* send the request sensor value stream command */
00618   if (vt_serial_write_byte(portfd,'S') < Ok)
00619     return(vt_print_error("request_sample_stream"));
00620 
00621   /* we don't wait for the response so we can do other things while */
00622   /* CGIU is sending data */
00623 
00624   return (Ok);
00625 }
00626 
00627 
00628 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00629 /* ENABLE_SWITCH_CONTROLS_LIGHT --                             */
00630 /*                                                             */
00631 /*                                                             */
00632 /* Args:                                                       */
00633 /*   CbGlove glove                                          */
00634 /*   Boolean enabled                                           */
00635 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00636 
00637 int
00638 CyberGloveBasic::enable_switch_controls_light(CbGlove glove, va_list temp)
00639 {
00640   int portfd = glove->portfd;
00641   CbGlovePrivate private_data = glove->private_data;
00642   Boolean enabled;
00643 
00644   enabled = va_arg(temp, int);
00645 
00646   /* send the switch controls light command */
00647   if ((vt_serial_write_byte(portfd,CG_LIGHTCNTL_CH) < Ok) ||
00648       (vt_serial_write_byte(portfd,enabled) < Ok))
00649     return(vt_print_error("enable_switch_controls_light"));
00650 
00651   /* check the response */
00652   if (command_reply_check(glove,CG_LIGHTCNTL_CH) < Ok)
00653     return(vt_print_error("enable_switch_controls_light"));
00654 
00655   private_data->param_flags.word = (enabled ?
00656               private_data->param_flags.word | CG_SWITCH_FLAG :
00657               private_data->param_flags.word & ~CG_SWITCH_FLAG);
00658   return (Ok);
00659 }
00660 
00661 
00662 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00663 /* SET_SENSOR_MASK --                                          */
00664 /*                                                             */
00665 /*                                                             */
00666 /* Args:                                                       */
00667 /*   CbGlove glove                                          */
00668 /*   unsigned long mask                                        */
00669 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00670 
00671 int
00672 CyberGloveBasic::set_sensor_mask(CbGlove glove, va_list temp)
00673 {
00674   int num_enabled_sensors = 0, portfd = glove->portfd;
00675   CbGlovePrivate private_data = glove->private_data;
00676   unsigned long mask;
00677   unsigned char *byte;
00678 
00679   /* send the set software sensor mask command */
00680   if (vt_serial_write_byte(portfd,CG_SENSOR_MASK_CH) < Ok)
00681     return(vt_print_error("set_sensor_mask"));
00682 
00683   mask = va_arg(temp, unsigned long);
00684   mask = mask & private_data->available_sensors.word;
00685 
00686   byte = (unsigned char *) &mask;
00687   /* send the mask */
00688   if ((vt_serial_write_byte(portfd,byte[LONG_BYTE_0]) < Ok) ||
00689       (vt_serial_write_byte(portfd,byte[LONG_BYTE_1]) < Ok) ||
00690       (vt_serial_write_byte(portfd,byte[LONG_BYTE_2]) < Ok))
00691     return(vt_print_error("set_sensor_mask"));
00692 
00693   /* check the response */
00694   if (command_reply_check(glove,CG_SENSOR_MASK_CH) < Ok)
00695     return(vt_print_error("set_sensor_mask"));
00696 
00697   private_data->sensors_in_use.word = mask;
00698 
00699   while (mask != 0)
00700   {
00701     if (mask & 0x1)
00702       num_enabled_sensors++;
00703     mask >>= 1;
00704   }
00705   private_data->num_enabled_sensors = num_enabled_sensors;
00706   /* probably should check to make sure num_sensor_returned */
00707   /* is <= num_enabled_sensors - LJE                        */
00708 
00709   return (Ok);
00710 }
00711 
00712 
00713 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00714 /* SET_NUM_VALUES_RETURNED --                                  */
00715 /*                                                             */
00716 /*                                                             */
00717 /* Args:                                                       */
00718 /*   CbGlove glove                                          */
00719 /*   int num_values                                            */
00720 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00721 
00722 int
00723 CyberGloveBasic::set_num_values_returned(CbGlove glove, va_list temp)
00724 {
00725   int portfd = glove->portfd;
00726   CbGlovePrivate private_data = glove->private_data;
00727   int num_values;
00728 
00729   /* send the set software sensor mask command */
00730   if (vt_serial_write_byte(portfd,CG_NUM_VALUES_CH) < Ok)
00731     return(vt_print_error("set_num_values_returned"));
00732 
00733   num_values = va_arg(temp, int);
00734   if (num_values > private_data->num_enabled_sensors)
00735     num_values = private_data->num_enabled_sensors;
00736 
00737   /* send the number of values desired */
00738   if (vt_serial_write_byte(portfd,(unsigned char) num_values) < Ok)
00739     return(vt_print_error("set_num_values_returned"));
00740 
00741   /* check the response */
00742   if (command_reply_check(glove,CG_NUM_VALUES_CH) < Ok)
00743     return(vt_print_error("set_num_values_returned"));
00744 
00745   return (Ok);
00746 }
00747 
00748 
00749 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00750 /* SET_PARAM_FLAGS --                                          */
00751 /*                                                             */
00752 /*                                                             */
00753 /* Args:                                                       */
00754 /*   CbGlove glove                                          */
00755 /*   unsigned long param_flags                                 */
00756 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00757 
00758 int
00759 CyberGloveBasic::set_param_flags(CbGlove glove, va_list temp)
00760 {
00761   int portfd = glove->portfd;
00762   CbGlovePrivate private_data = glove->private_data;
00763   word_byte_union param_flags;
00764 
00765   /* send the set software sensor mask command */
00766   if (vt_serial_write_byte(portfd,CG_PARAMETER_CH) < Ok)
00767     return(vt_print_error("set_param_flags"));
00768 
00769   param_flags.word = va_arg(temp, unsigned long);
00770 
00771   /* send the flags */
00772   if ((vt_serial_write_byte(portfd,param_flags.byte[LONG_BYTE_0]) < Ok) ||
00773       (vt_serial_write_byte(portfd,param_flags.byte[LONG_BYTE_1]) < Ok) ||
00774       (vt_serial_write_byte(portfd,param_flags.byte[LONG_BYTE_2]) < Ok))
00775     return(vt_print_error("set_param_flags"));
00776 
00777   /* check the response */
00778   if (command_reply_check(glove,CG_PARAMETER_CH) < Ok)
00779     return(vt_print_error("set_param_flags"));
00780 
00781   private_data->param_flags.word = param_flags.word;
00782 
00783   return (Ok);
00784 }
00785 
00786 
00787 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00788 /* REBOOT_CGIU --                                              */
00789 /*                                                             */
00790 /*                                                             */
00791 /* Args:                                                       */
00792 /*   CbGlove glove                                          */
00793 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00794 
00795 int
00796 CyberGloveBasic::reboot_CGIU(CbGlove glove, va_list temp)
00797 {
00798   int lastbyte, portfd = glove->portfd;
00799   unsigned char buffer[2];
00800 
00801   /* send the request sensor value command */
00802   if (vt_serial_write_byte(portfd,CG_REBOOT_CH) < Ok)
00803     return (vt_print_error("reboot_CGIU"));
00804 
00805   delay(REBOOT_WAIT);                      /*  wait for system to boot */
00806   /* check the response */
00807   if (vt_serial_read_byte(glove->portfd) != CG_REBOOT_CH)
00808     return (vt_set_error("reboot_CGIU",CG_ERROR1));
00809   if (vt_serial_read_bytes(glove->portfd,buffer,2) < Ok)
00810     return(vt_print_error("reboot_CGIU"));
00811 
00812   if (strncmp((const char *)buffer,"\r\n",2) != 0)
00813     return (vt_set_error("reboot_CGIU",CG_ERROR5));
00814 
00815   if ((lastbyte = vt_serial_read_byte(glove->portfd)) == -1)
00816     return(vt_print_error("reboot_CGIU"));
00817 
00818   if (lastbyte != CG_TERMINATOR)
00819     return (vt_set_error("reboot_CGIU",CG_ERROR5));
00820 
00821   return (Ok);
00822 }
00823 
00824 
00825 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00826 /* VT_SEND_GLOVE_QUERY --                                      */
00827 /*                                                             */
00828 /*                                                             */
00829 /* Args:                                                       */
00830 /*   int id  (glove command id, see enums in vt_glove-types.h) */
00831 /*   any other args required by specific command, see above    */
00832 /*   routines                                                  */
00833 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00834 
00835 int
00836 CyberGloveBasic::vt_send_glove_command(int id, ...)
00837 {
00838   va_list address_arg1;
00839   va_start(address_arg1,id);
00840 
00841   static int (CyberGloveBasic::*command_function[])(CbGlove, va_list) =
00842   {
00843     &CyberGloveBasic::set_baud_rate, NULL, NULL, &CyberGloveBasic::enable_timestamp,
00844     &CyberGloveBasic::enable_filter, &CyberGloveBasic::request_single_sample,
00845     &CyberGloveBasic::request_sample_stream, NULL,
00846     &CyberGloveBasic::enable_switch_controls_light, NULL,
00847     &CyberGloveBasic::set_sensor_mask, &CyberGloveBasic::set_num_values_returned,
00848     &CyberGloveBasic::set_param_flags, &CyberGloveBasic::reboot_CGIU, NULL, NULL, NULL,
00849     NULL, NULL
00850   };
00851 
00852   if (current_glove->portfd == -1)
00853     return (Ok);
00854 
00855   if ((this->*command_function[id])(current_glove,address_arg1) < Ok)
00856     return (vt_print_error("vt_send_glove_command"));
00857 
00858   return (Ok);
00859 }
00860 
00861 
00862 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00863 /*                                                             */
00864 /*  routines to query the glove                                */
00865 /*                                                             */
00866 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00867 
00868 
00869 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00870 /* GLOVE_OK_QUERY --                                           */
00871 /*                                                             */
00872 /*                                                             */
00873 /* Args:                                                       */
00874 /*   CbGlove glove                                          */
00875 /*   Boolean *status_return                                    */
00876 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00877 
00878 int
00879 CyberGloveBasic::glove_ok_query(CbGlove glove, va_list temp)
00880 {
00881   int status, portfd = glove->portfd;
00882   CbGlovePrivate private_data = glove->private_data;
00883   char *query_string = CG_GLOVE_QUERY;
00884   unsigned long flags;
00885   Boolean *status_return = NULL;
00886 
00887   if (temp)
00888     status_return = va_arg(temp,Boolean *);
00889 
00890   if (status_return)
00891     *status_return = 0;
00892 
00893   query_string[CG_CMD_BYTE] = CG_GLOVE_OK_QUERY_CH;
00894   /* send the command */
00895   if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
00896     return (vt_print_error("glove_ok_query"));
00897 
00898   /* check for a bad header returned */
00899   if (query_reply_head_check(portfd,query_string) < Ok)
00900     return (vt_print_error("glove_ok_query"));
00901 
00902   /* read the data returned */
00903   if ((status = vt_serial_read_byte(portfd)) == -1)
00904     return (vt_print_error("glove_ok_query"));
00905 
00906   /* check for a bad tail byte returned */
00907   if (reply_tail_check(glove) < Ok)
00908     return (vt_print_error("glove_ok_query"));
00909 
00910   if (status_return)
00911     *status_return = status;
00912 
00913   flags = private_data->param_flags.word;
00914   flags = (status & CG_VALID_INIT) ? (flags | CG_GLOVE_VALID_FLAG) :
00915                                      (flags & ~CG_GLOVE_VALID_FLAG);
00916   flags = (status & CG_GLOVE_CONNECTED) ? (flags | CG_GLOVE_IN_FLAG) :
00917                                           (flags & ~CG_GLOVE_IN_FLAG);
00918   private_data->param_flags.word = flags;
00919 
00920   return (Ok);
00921 }
00922 
00923 
00924 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00925 /* GET_GLOVE_INFO -                                            */
00926 /*                                                             */
00927 /*                                                             */
00928 /* Args:                                                       */
00929 /*   CbGlove glove                                          */
00930 /*   char **info_return                                        */
00931 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00932 
00933 int
00934 CyberGloveBasic::get_glove_info(CbGlove glove, va_list temp)
00935 {
00936   int portfd = glove->portfd;
00937   char *query_string = CG_GLOVE_QUERY;
00938   static char buffer[2048];
00939   char **info_return = NULL;
00940 
00941   if (temp)
00942     info_return = va_arg(temp, char **);
00943   /* if they didn't pass us a pointer we might as well return */
00944   /* since we don't save the info in the glove structure */
00945   if (!info_return)
00946     return (Ok);
00947   *info_return = buffer;
00948   buffer[0] = 0;
00949 
00950   query_string[CG_CMD_BYTE] = CG_INFO_QUERY_CH;
00951   /* request the glove info */
00952   if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
00953     return (vt_print_error("get_glove_info"));
00954 
00955   /* check for a bad header returned */
00956   if (query_reply_head_check(portfd,query_string) < Ok)
00957     return (vt_print_error("get_glove_info"));
00958 
00959   /* wait for the info to be ready */
00960   delay(STATUS_WAIT);
00961 
00962   /* now read the info */
00963   if (vt_serial_read_bytes(portfd,(unsigned char *)buffer,2048) < Ok)
00964   {
00965     buffer[0] = 0;
00966     return (vt_print_error("get_glove_info"));
00967   }
00968 
00969   /* check for a bad tail byte returned */
00970   if (reply_tail_check(glove) < Ok)
00971   {
00972     buffer[0] = 0;
00973     return (vt_print_error("get_glove_info"));
00974   }
00975 
00976   return (Ok);
00977 }
00978 
00979 
00980 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00981 /* GET_AVAIL_SENSOR_MASK --                                    */
00982 /*                                                             */
00983 /*                                                             */
00984 /* Args:                                                       */
00985 /*   CbGlove glove                                          */
00986 /*   unsigned long *mask_return                                */
00987 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00988 
00989 int
00990 CyberGloveBasic::get_avail_sensor_mask(CbGlove glove, va_list temp)
00991 {
00992   int portfd = glove->portfd;
00993   CbGlovePrivate private_data = glove->private_data;
00994   char *query_string = CG_GLOVE_QUERY;
00995   unsigned char mask[3];
00996   unsigned long *mask_return;
00997 
00998   if (temp)
00999     mask_return = va_arg(temp,unsigned long *);
01000 
01001   if (mask_return)
01002     *mask_return = 0;
01003 
01004   query_string[CG_CMD_BYTE] = CG_HARD_MASK_QUERY_CH;
01005   /* request the hardware sensor mask */
01006   if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
01007     return (vt_print_error("get_avail_sensor_mask"));
01008 
01009   /* check for a bad header returned */
01010   if (query_reply_head_check(portfd,query_string) < Ok)
01011     return (vt_print_error("get_avail_sensor_mask"));
01012 
01013   /* if we got the appropriate responses read the mask */
01014   if (vt_serial_read_bytes(portfd,mask,3) < Ok)
01015     return (vt_print_error("get_avail_sensor_mask"));
01016 
01017   /* check for a bad tail byte returned */
01018   if (reply_tail_check(glove) < Ok)
01019     return (vt_print_error("get_avail_sensor_mask"));
01020 
01021   private_data->available_sensors.byte[LONG_BYTE_0] = mask[0];
01022   private_data->available_sensors.byte[LONG_BYTE_1] = mask[1];
01023   private_data->available_sensors.byte[LONG_BYTE_2] = mask[2];
01024 
01025   if (mask_return)
01026     *mask_return = private_data->available_sensors.word;
01027 
01028   return (Ok);
01029 }
01030 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01031 /* GET_SENSOR_MASK --                                          */
01032 /*                                                             */
01033 /*                                                             */
01034 /* Args:                                                       */
01035 /*   CbGlove glove                                          */
01036 /*   unsigned long *mask_return                                */
01037 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01038 
01039 int
01040 CyberGloveBasic::get_sensor_mask(CbGlove glove, va_list temp)
01041 {
01042   int num_enabled_sensors = 0, portfd = glove->portfd;
01043   CbGlovePrivate private_data = glove->private_data;
01044   char *query_string = CG_GLOVE_QUERY;
01045   word_byte_union mask;
01046   unsigned long *mask_return = NULL;
01047 
01048   if (temp)
01049     mask_return = va_arg(temp,unsigned long *);
01050 
01051   if (mask_return)
01052     *mask_return = 0;
01053 
01054   query_string[CG_CMD_BYTE] = CG_SENSOR_MASK_CH;
01055   /* request the hardware sensor mask */
01056   if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
01057     return (vt_print_error("get_sensor_mask"));
01058 
01059   /* check the response */
01060   if (query_reply_head_check(portfd,query_string) < Ok)
01061     return (vt_print_error("get_sensor_mask"));
01062 
01063   /* if we got the appropriate responses read the mask */
01064   if (vt_serial_read_bytes(portfd,mask.byte,3) < Ok)
01065     return (vt_print_error("get_sensor_mask"));
01066 
01067   /* check for a bad tail byte returned */
01068   if (reply_tail_check(glove) < Ok)
01069     return (vt_print_error("get_sensor_mask"));
01070 
01071   private_data->sensors_in_use.byte[LONG_BYTE_0] = mask.byte[0];
01072   private_data->sensors_in_use.byte[LONG_BYTE_1] = mask.byte[1];
01073   private_data->sensors_in_use.byte[LONG_BYTE_2] = mask.byte[2];
01074 
01075   if (mask_return)
01076     *mask_return = private_data->sensors_in_use.word;
01077 
01078   /* just to make sure our idea of how many values come back is right... */
01079   while (mask.word != 0)
01080   {
01081     if (mask.word & 0x1)
01082       num_enabled_sensors++;
01083     mask.word >>= 1;
01084   }
01085   private_data->num_enabled_sensors = num_enabled_sensors;
01086   /* LJE check to make sure num_sensor_returned is <= num_enabled_sensors */
01087 
01088   return (Ok);
01089 }
01090 
01091 
01092 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01093 /* GET_NUM_VALUES_RETURNED --                                  */
01094 /*                                                             */
01095 /*                                                             */
01096 /* Args:                                                       */
01097 /*   CbGlove glove                                          */
01098 /*   int *num_values_return                                    */
01099 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01100 
01101 int
01102 CyberGloveBasic::get_num_values_returned(CbGlove glove, va_list temp)
01103 {
01104   int portfd = glove->portfd;
01105   CbGlovePrivate private_data = glove->private_data;
01106   char *query_string = CG_GLOVE_QUERY;
01107   int num_values;
01108   int *num_values_return = NULL;
01109 
01110   if (temp)
01111     num_values_return = va_arg(temp, int *);
01112 
01113   if (num_values_return)
01114     *num_values_return = 0;
01115 
01116   query_string[CG_CMD_BYTE] = CG_NUM_VALUES_CH;
01117   /* query how many sensor values are returned */
01118   if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
01119     return (vt_print_error("get_num_values_returned"));
01120 
01121   /* check the response */
01122   if (query_reply_head_check(portfd,query_string) < Ok)
01123     return (vt_print_error("get_num_values_returned"));
01124 
01125   if ((num_values = vt_serial_read_byte(portfd)) == -1)
01126     return (vt_print_error("get_num_values_returned"));
01127 
01128   /* check for a bad tail byte returned */
01129   if (reply_tail_check(glove) < Ok)
01130     return (vt_print_error("get_num_values_returned"));
01131 
01132   private_data->num_values_returned = num_values;
01133 
01134   if (num_values_return)
01135     *num_values_return = num_values;
01136 
01137   return (Ok);
01138 }
01139 
01140 
01141 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01142 /* READ_PARAM_FLAGS --                                         */
01143 /*                                                             */
01144 /*                                                             */
01145 /* Args:                                                       */
01146 /*   CbGlove glove                                          */
01147 /*   unsigned long *param_return                               */
01148 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01149 
01150 int
01151 CyberGloveBasic::read_param_flags(CbGlove glove, va_list temp)
01152 {
01153   int portfd = glove->portfd;
01154   CbGlovePrivate private_data = glove->private_data;
01155   char *query_string = CG_GLOVE_QUERY;
01156   unsigned char param[3];
01157   unsigned long *param_return = NULL;
01158 
01159   if (temp)
01160     param_return = va_arg(temp,unsigned long *);
01161 
01162   if (param_return)
01163     *param_return = 0;
01164 
01165   query_string[CG_CMD_BYTE] = CG_PARAMETER_CH;
01166   /* request the parameter flags */
01167   if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
01168     return (vt_print_error("read_param_flags"));
01169 
01170   /* check the response */
01171   if (query_reply_head_check(portfd,query_string) < Ok)
01172     return (vt_print_error("read_param_flags"));
01173 
01174   /* if we got the appropriate responses read the parameter flags */
01175   if (vt_serial_read_bytes(portfd,param,3) < Ok)
01176     return (vt_print_error("read_param_flags"));
01177 
01178   /* check for a bad tail byte returned */
01179   if (reply_tail_check(glove) < Ok)
01180     return (vt_print_error("read_param_flags"));
01181 
01182   private_data->param_flags.byte[LONG_BYTE_0] = param[0];
01183   private_data->param_flags.byte[LONG_BYTE_1] = param[1];
01184   private_data->param_flags.byte[LONG_BYTE_2] = param[2];
01185   if (param_return)
01186     *param_return = private_data->param_flags.word;
01187 
01188   return (Ok);
01189 }
01190 
01191 
01192 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01193 /* RIGHT_HAND_GLOVE_QUERY --                                   */
01194 /*                                                             */
01195 /*                                                             */
01196 /* Args:                                                       */
01197 /*   CbGlove glove                                          */
01198 /*   Boolean *righthand_return                                 */
01199 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01200 
01201 int
01202 CyberGloveBasic::right_hand_glove_query(CbGlove glove, va_list temp)
01203 {
01204   int portfd = glove->portfd;
01205   CbGlovePrivate private_data = glove->private_data;
01206   char *query_string = CG_GLOVE_QUERY;
01207   unsigned long flags;
01208   int is_righthand;
01209   Boolean *righthand_return = NULL;
01210 
01211   if (temp)
01212     righthand_return = va_arg(temp,Boolean *);
01213 
01214   if (righthand_return)
01215     *righthand_return = 0;
01216 
01217   query_string[CG_CMD_BYTE] = CG_RIGHT_HAND_QUERY_CH;
01218   /* send the command */
01219   if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
01220     return (vt_print_error("right_hand_glove_query"));
01221 
01222   /* check for a bad header returned */
01223   if (query_reply_head_check(portfd,query_string) < Ok)
01224     return (vt_print_error("right_hand_glove_query"));
01225 
01226   /* read the data returned */
01227   if ((is_righthand = vt_serial_read_byte(portfd)) == -1)
01228     return (vt_print_error("right_hand_glove_query"));
01229 
01230   /* check for a bad tail byte returned */
01231   if (reply_tail_check(glove) < Ok)
01232     return (vt_print_error("right_hand_glove_query"));
01233 
01234   if (righthand_return)
01235     *righthand_return = is_righthand;
01236 
01237   flags = private_data->param_flags.word;
01238   flags = (is_righthand) ? (flags | CG_RIGHT_HAND_FLAG) :
01239                            (flags & ~CG_RIGHT_HAND_FLAG);
01240   private_data->param_flags.word = flags;
01241 
01242   return (Ok);
01243 }
01244 
01245 
01246 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01247 /* GET_MAX_NUM_SENSORS --                                      */
01248 /*                                                             */
01249 /*                                                             */
01250 /* Args:                                                       */
01251 /*   CbGlove glove                                          */
01252 /*   int *num_sensors_return                                   */
01253 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01254 
01255 int
01256 CyberGloveBasic::get_max_num_sensors(CbGlove glove, va_list temp)
01257 {
01258   int portfd = glove->portfd;
01259   CbGlovePrivate private_data = glove->private_data;
01260   char *query_string = CG_GLOVE_QUERY;
01261   int num_sensors;
01262   int *num_sensors_return = NULL;
01263 
01264   if (temp)
01265     num_sensors_return = va_arg(temp,int *);
01266 
01267   if (num_sensors_return)
01268     *num_sensors_return = 0;
01269 
01270   query_string[CG_CMD_BYTE] = CG_MAX_SENSORS_QUERY_CH;
01271   /* send the command */
01272   if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
01273     return (vt_print_error("get_max_num_sensors"));
01274 
01275   /* check for a bad header returned */
01276   if (query_reply_head_check(portfd,query_string) < Ok)
01277     return (vt_print_error("get_max_num_sensors"));
01278 
01279   /* read the data returned */
01280   if ((num_sensors = vt_serial_read_byte(portfd)) == -1)
01281     return (vt_print_error("get_max_num_sensors"));
01282 
01283   /* check for a bad tail byte returned */
01284   if (reply_tail_check(glove) < Ok)
01285     return (vt_print_error("get_max_num_sensors"));
01286 
01287   if (num_sensors_return)
01288     *num_sensors_return = num_sensors;
01289 
01290   private_data->num_sensors_available = num_sensors;
01291   return Ok;
01292 }
01293 
01294 
01295 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01296 /* VT_SEND_GLOVE_QUERY --                                      */
01297 /*                                                             */
01298 /*                                                             */
01299 /* Args:                                                       */
01300 /*   int id  (query command id see enums in vt_glove-types.h)  */
01301 /*   any other args required by specific command, see above    */
01302 /*   routines                                                  */
01303 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
01304 
01305 int
01306 CyberGloveBasic::vt_send_glove_query(int id, ...)
01307 {
01308   va_list address_arg1;
01309   va_start(address_arg1, id);
01310 
01311   static int (CyberGloveBasic::*query_function[])(CbGlove, va_list) =
01312   {
01313     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
01314     &CyberGloveBasic::get_sensor_mask, &CyberGloveBasic::get_num_values_returned,
01315     &CyberGloveBasic::read_param_flags, NULL, NULL, NULL, NULL, NULL,
01316     NULL, &CyberGloveBasic::glove_ok_query, &CyberGloveBasic::get_glove_info,
01317     &CyberGloveBasic::get_avail_sensor_mask, &CyberGloveBasic::right_hand_glove_query,
01318     &CyberGloveBasic::get_max_num_sensors
01319   };
01320 
01321   if (current_glove->portfd == -1)
01322     return (Ok);
01323 
01324   if ((this->*query_function[id])(current_glove,address_arg1) < Ok)
01325     return (vt_print_error("vt_send_glove_query"));
01326 
01327   va_end(address_arg1);
01328 
01329   return (Ok);
01330 }
01331 
01332 
01333 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
01334 /*                                                             */
01335 /*  routines to process the raw glove values                   */
01336 /*                                                             */
01337 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
01338 
01339 
01340 void
01341 CyberGloveBasic::vt_process_glove_data(void)
01342 {
01343   int finger, joint;
01344   glove_sensor_map_struct (*mapping)[MAX_GROUP_VALUES];
01345   volatile float (*angle)[MAX_GROUP_VALUES];
01346   unsigned char (*raw_value)[MAX_GROUP_VALUES];
01347 
01348   mapping = current_glove_private->mapping;
01349   angle = current_glove->joint_angle;
01350   raw_value = current_glove->raw_value;
01351 
01352   /* NOTE: we negate gain here, so that sense of rotations is appropriate */
01353   /* for the digit coordinate systems                                     */
01354   for (finger = THUMB; finger < FINGERS+1; finger++)
01355     for (joint = MCP; joint < JOINTS; joint++)
01356       angle[finger][joint] = (raw_value[finger][joint] -
01357                mapping[finger][joint].offset) *
01358             -mapping[finger][joint].gain;
01359 
01360   /*  kludge limiter for back of thumb rotation sensor  */
01361   if (angle[THUMB][MCP] > 0.5)
01362     angle[THUMB][MCP] = 0.5;
01363 
01364   //  current_glove_private->abduct_fingers();
01365   abduct_18();
01366 
01367   /* if we have an 18 sensor glove we fake the distal flex */
01368   if (current_glove_private->num_sensors_available <= 18)
01369     for (finger = INDEX; finger < FINGERS; finger++)
01370     {
01371       angle[finger][DIP] = -(angle[finger][PIP] * angle[finger][PIP] * 0.32 +
01372               angle[finger][PIP] * 0.17);
01373     }
01374 
01375   /* if we have a left handed glove negate abduction angles */
01376   if (!(current_glove_private->param_flags.word & CG_RIGHT_HAND_FLAG))
01377   {
01378     for (finger = THUMB; finger < FINGERS; finger++)
01379       angle[finger][ABDUCT] = -angle[finger][ABDUCT];
01380 
01381     angle[CARPUS][WRIST_YAW] = -angle[CARPUS][WRIST_YAW];
01382   }
01383 }
01384 
01385 void
01386 CyberGloveBasic::abduct_18(void)
01387 {
01388   /*  Abduct function for gloves w/MDL,RNG,PNK sensors only              */
01389   /*  This algorithm allows the Index to be set directly with the        */
01390   /*    index gain slider.  The amount of middle finger bias is          */
01391   /*    set with the middle gain slider.  Offsets should be taken        */
01392   /*    with the fingers inline with each respective metacarpal.         */
01393   /*    Each finger gain should be set, while holding all others         */
01394   /*    in their offset position.                                        */
01395   glove_sensor_map_struct (*mapping)[MAX_GROUP_VALUES];
01396   float index_abduct,middle_abduct;
01397   volatile float (*angle)[MAX_GROUP_VALUES];
01398   unsigned char (*raw_value)[MAX_GROUP_VALUES];
01399 
01400   mapping = current_glove_private->mapping;
01401   angle = current_glove->joint_angle;
01402   raw_value = current_glove->raw_value;
01403 
01404   index_abduct = (raw_value[MIDDLE][ABDUCT] - mapping[MIDDLE][ABDUCT].offset) *
01405               mapping[INDEX][ABDUCT].gain;
01406   middle_abduct = -(angle[RING][ABDUCT] + index_abduct) *
01407                         mapping[MIDDLE][ABDUCT].gain;
01408 
01409   angle[INDEX][ABDUCT] = index_abduct + middle_abduct;
01410   angle[RING][ABDUCT] += middle_abduct;
01411   angle[PINKY][ABDUCT] += angle[RING][ABDUCT];
01412   angle[MIDDLE][ABDUCT] = middle_abduct;
01413 }
