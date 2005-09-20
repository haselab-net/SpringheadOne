 /*  FILE: vt_read_user_cal.c  --                                */
00002 /*                                                              */
00003 /*  Routines to read and write hand calibration/geometry data   */
00004 /*                                                              */
00005 /*  ==========================================================  */
00006 /*  --  Copyright (C) 1990,91,92,93 Virtual Technologies    --  */
00007 /*  --                                                      --  */
00008 /*  --    Author: William Chapin                            --  */
00009 /*  --    Modified by Larry Edwards                         --  */
00010 /*  ==========================================================  */
00011 
00012 #include <gadget/Devices/DriverConfig.h>
00013 #include <stdio.h>
00014 
00015 #include <X11/Intrinsic.h>
00016 
00017 #include <gadget/Devices/VirtualTechnologies/CyberGlove/CyberGlove.h>
00018 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_read_user_cal-types.h>
00019 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_virtual_hand-types.h>
00020 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_globals.h>
00021 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_error.h>
00022 
00023 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00024 /*                                                               */
00025 /*  Private Global Variables                                     */
00026 /*                                                               */
00027 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00028 
00029 /* comments and version number for writing out the calibration/geometry file */
00030 
00031 static char *geo_header[] =
00032 {
00033   "! ==",
00034   "3D HAND GEOMETRY follows:",
00035   "! ==",
00036   "! NOTE:",
00037   "!     The geometry of the system axes is to change in",
00038   "!   in future releases to account for the roll of the arm",
00039   "!   to equate to roll of about the X-axis.  The Y-axis",
00040   "!   will then be through the wrist from right to left.",
00041   "!   The Z-axis will always be up.",
00042   "! ==",
00043   "! This geometry is for a:",
00044   ""
00045   };
00046 
00047 static char *sensor_header[] =
00048 {
00049   "!   The handedness can be changed by flipping the geometry",
00050   "!   across its axis of symmetry.",
00051   "!   * Currently this is { x = -x }, but it will be { y = -y }",
00052   "!     with the X-axis as the axis of symmetry.",
00053   "! ==",
00054   "! backbone of hand -- flat structural geometry (centimeters)",
00055   "! ==",
00056   "!       wrist_offset  =  offset of wrist's center of rotation",
00057   "!    x      y      z        relative to tracker receiver.",
00058   ""
00059   };
00060 
00061 static char *hand_header[] =
00062 {
00063   "! ==",
00064   "! # of fingers, # of vertices/finger, thumb-aspect and hand-roll",
00065   "!       thumb-roll = roll angle of thumb interphalangeal bend axis",
00066   "!       hand-roll  = roll angle of wrist pitch axis",
00067   "!    in degrees relative to plane of metacarpals.",
00068   ""
00069   };
00070 
00071 static char *finger_header[] =
00072 {
00073   "! ==",
00074   "! Each finger is with <x, y, z> joint/tip position relative to",
00075   "!   the plane of the metacarpals (unrolled) with origin=wrist",
00076   "!   the initial integer is currently used as a VGA color reference",
00077   "!   the fourth real is the finger diameter at the joint.",
00078   ""
00079   };
00080 
00081 static char *carp_header[] =
00082 {
00083   "! ==",
00084   "! There will be several parameters describing the",
00085   "!   carpals.  Presently only one point is used.",
00086   "! pisiform carpal bone = the bone that the hand rests on",
00087   ""
00088   };
00089 
00090 static char *forearm_header[] =
00091 {
00092   "! ==",
00093   "! radial-carpus, ulna-carpus at the wrist joint",
00094   "! radial-cranon, olecranon, and ulna-coronoid points at elbow joint",
00095   ""
00096   };
00097 
00098 static char *thumb_roll_axis_header[] =
00099 {
00100   "! Thumb roll axis. This is the vector the thumb rolls about as it abducts",
00101   "! and rotates about the MCP joint",
00102   "! Fairly good results are had just using the vector from the thumb MCP",
00103   "! joint to the pinky MCP joint",
00104   ""
00105 };
00106 
00107 static char *thumb_mcp_axis_header[] =
00108 {
00109   "! MCP rotation axis. Nominally the vector from the thumb"
00110     " MCP joint to the index",
00111     "! finger MCP joint",
00112     ""
00113 };
00114 
00115 
00116 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00117 /*                                                               */
00118 /*  Low level support routines                                   */
00119 /*                                                               */
00120 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00121 
00122 
00123 void
00124 skip_comments(FILE *inputfp, char input_line[])
00125 {
00126   while (fgets(input_line, 81, inputfp))
00127   {
00128     if (input_line[0] != '!')
00129       break;
00130   }
00131 
00132 }
00133 
00134 static void
00135 print_list(FILE *outputfp, char **list)
00136 {
00137   char **ptr;
00138   for (ptr = list ; **ptr != '\0' ; ptr++)
00139     fprintf(outputfp, "%s\n", *ptr);
00140 }
00141 
00142 
00143 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00144 /*                                                               */
00145 /*  Routines to read and write the hand geometry                 */
00146 /*                                                               */
00147 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00148 
00149 
00150 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00151 /*  VT_GEOM_IN --                                                */
00152 /*                                                               */
00153 /*    Read in the user's geometry from the file                  */
00154 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00155 
00156 int
00157 CyberGloveBasic::vt_geom_in(FILE *inputfp, UserGeometry user)
00158 {
00159   int i, finger, joint, finger_color;
00160   char input_line[82];
00161   static char geom_header[] = "3D HAND GEOMETRY";
00162   vec3d tempvec[5];
00163   Boolean found_geom = FALSE;
00164 
00165   rewind(inputfp);      /* make sure we're at beginning of file */
00166 
00167   while (fgets(input_line, 81, inputfp))
00168     if ((found_geom =
00169      (strncmp(input_line,geom_header,sizeof(geom_header)-1) == 0)))
00170       break;
00171   if (!found_geom)
00172     return (vt_set_error("vt_read_calibration",CAL_ERROR1));
00173 
00174   skip_comments(inputfp,input_line);
00175   if (input_line[0] != ' ')
00176     user->right_hand = (input_line[0] == 'R');
00177 
00178   skip_comments(inputfp,input_line);
00179   /* read wrist offset */
00180   sscanf(input_line, "%f %f %f", &user->wrist_offset[VX],
00181      &user->wrist_offset[VY],&user->wrist_offset[VZ]);
00182 
00183   skip_comments(inputfp,input_line);
00184   sscanf(input_line, "%d %d %f %f", &user->fingers, &user->joints,
00185      &user->thumb_roll, &user->hand_roll);
00186   user->thumb_roll *= DEG2RAD;  /*  conversion to radians  */
00187   user->hand_roll *= DEG2RAD;   /*  conversion to radians  */
00188 
00189   for (finger = THUMB; finger < user->fingers; finger++)
00190   {
00191     skip_comments(inputfp,input_line);
00192     sscanf(input_line, "%d", &finger_color);
00193     for (joint = MCP; joint < user->joints; joint++)
00194     {
00195       fgets(input_line, 81, inputfp);
00196       sscanf(input_line, "%f %f %f %f",
00197          &user->geom[finger][joint][VX], &user->geom[finger][joint][VY],
00198          &user->geom[finger][joint][VZ],
00199          &user->knuckle_radius[finger][joint]);
00200     }
00201   }
00202 
00203   skip_comments(inputfp,input_line);
00204   sscanf(input_line, "%f %f %f",
00205      &user->pisiform[VX], &user->pisiform[VY], &user->pisiform[VZ]);
00206 
00207   /*  The following kludge just goes to show you that the geometry */
00208   /*  file format is due to be upgraded.                           */
00209   for (i = 0 ; i < 5 ; i++)
00210   {
00211     skip_comments(inputfp,input_line);
00212     sscanf(input_line, "%f %f %f",
00213        &tempvec[i][VX], &tempvec[i][VY], &tempvec[i][VZ]);
00214   }
00215   vt_copy_vec3(tempvec[0],user->radi_ulna[0]);
00216   user->radi_ulna[0][VZ] -= 1.0;
00217   vt_copy_vec3(tempvec[0],user->radi_ulna[1]);
00218   user->radi_ulna[1][VZ] += 1.0;
00219 
00220   vt_copy_vec3(tempvec[1],user->radi_ulna[2]);
00221   vt_copy_vec3(tempvec[1],user->radi_ulna[3]);
00222   user->radi_ulna[3][VZ] -= 2.0;
00223   vt_copy_vec3(tempvec[2],user->radi_ulna[4]);
00224   vt_copy_vec3(tempvec[3],user->radi_ulna[5]);
00225   vt_copy_vec3(tempvec[4],user->radi_ulna[6]);
00226   vt_copy_vec3(tempvec[3],user->radi_ulna[7]);
00227   user->radi_ulna[7][VY] += 5.0;
00228   user->radi_ulna[7][VZ] += 5.0;
00229 
00230   /* read thumb roll and mcp vectors */
00231   skip_comments(inputfp,input_line);
00232   sscanf(input_line, "%f %f %f",&user->thumb_roll_axis[VX],
00233      &user->thumb_roll_axis[VY], &user->thumb_roll_axis[VZ]);
00234   skip_comments(inputfp,input_line);
00235   sscanf(input_line, "%f %f %f", &user->thumb_mcp_axis[VX],
00236      &user->thumb_mcp_axis[VY], &user->thumb_mcp_axis[VZ]);
00237 
00238   return (Ok);
00239 }
00240 
00241 void
00242 CyberGloveBasic::vt_geom_out( FILE *outputfp, UserGeometry user)
00243 {
00244   int i, finger, joint;
00245 
00246   vec3d tempvec[5];
00247   char *finger_name[] = {"Thumb","Index","Middle","Ring","Pinky","WCarp"};
00248 
00249   print_list(outputfp, geo_header);
00250   fprintf(outputfp, "%s HAND.\n",(user->right_hand ? "RIGHT" : "LEFT" ));
00251 
00252   print_list(outputfp, sensor_header);
00253   fprintf(outputfp, "%6.2f %6.2f %6.2f\n",user->wrist_offset[VX],
00254       user->wrist_offset[VY], user->wrist_offset[VZ]);
00255 
00256   print_list(outputfp, hand_header);
00257   fprintf(outputfp, "%6d %6d %6.2f %6.2f\n",
00258       user->fingers, user->joints,
00259       user->thumb_roll*RAD2DEG, user->hand_roll*RAD2DEG);
00260 
00261   print_list(outputfp, finger_header);
00262   for (finger = THUMB; finger < user->fingers ; finger++)
00263   {
00264     fprintf(outputfp, "! %s\n", finger_name[finger]);
00265     fprintf(outputfp, "%d\n",1); /* we don't use the finger color field now */
00266     for (joint = MCP; joint < user->joints ; joint++)
00267     {
00268       fprintf(outputfp, "%6.2f %6.2f %6.2f %6.2f\n",
00269           user->geom[finger][joint][VX],
00270           user->geom[finger][joint][VY],
00271           user->geom[finger][joint][VZ],
00272           user->knuckle_radius[finger][joint]);
00273     }
00274   }
00275 
00276   print_list(outputfp, carp_header);
00277   fprintf(outputfp, "%6.2f %6.2f %6.2f\n",user->pisiform[VX],
00278       user->pisiform[VY], user->pisiform[VZ]);
00279 
00280   print_list(outputfp, forearm_header);
00281   vt_copy_vec3(user->radi_ulna[0],tempvec[0]);
00282   tempvec[0][VZ] += 1.0;
00283   vt_copy_vec3(user->radi_ulna[2],tempvec[1]);
00284   vt_copy_vec3(user->radi_ulna[4],tempvec[2]);
00285   vt_copy_vec3(user->radi_ulna[5],tempvec[3]);
00286   vt_copy_vec3(user->radi_ulna[6],tempvec[4]);
00287 
00288   for ( i = 0 ; i < 5 ; i++)
00289   {
00290     fprintf(outputfp, "%6.2f %6.2f %6.2f\n",
00291         tempvec[i][VX], tempvec[i][VY], tempvec[i][VZ]);
00292   }
00293 
00294   print_list(outputfp, thumb_roll_axis_header);
00295   fprintf(outputfp, "%6.2f %6.2f %6.2f\n",user->thumb_roll_axis[VX],
00296       user->thumb_roll_axis[VY], user->thumb_roll_axis[VZ]);
00297 
00298   print_list(outputfp, thumb_mcp_axis_header);
00299   fprintf(outputfp, "%6.2f %6.2f %6.2f\n",user->thumb_mcp_axis[VX],
00300       user->thumb_mcp_axis[VY], user->thumb_mcp_axis[VZ]);
00301 
00302   fprintf(outputfp, "! E_O_GEO\n!\n");
00303   fflush(outputfp);
00304 }
00305 
00306 
00307 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
00308 /*                                                                */
00309 /*  Routines to read and write calibration data for glove sensors */
00310 /*                                                                */
00311 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
00312 
00313 
00314 int
00315 CyberGloveBasic::vt_read_glove_calibration(FILE *inputfp, CbGlove glove)
00316 {
00317   int finger,joint,temp;
00318   char input_line[128];
00319   static char glove_cal_header[] = "GLOVE CALIBRATION";
00320   int offset;
00321   float version,gain;
00322   Boolean found_cal;
00323 
00324   rewind(inputfp);  /* make sure we're at beginning of file */
00325 
00326   fgets(input_line,81,inputfp);
00327   if (sscanf(input_line,"VHv%f.%*d",&version) < 1)
00328     printf("\n\n          Old format VH file ... please update.\n\n");
00329 
00330   /* look for the beginning of the hand calibration data */
00331   while (fgets(input_line,81,inputfp))
00332     if ((found_cal = (strncmp(input_line,glove_cal_header,
00333                  sizeof(glove_cal_header)-1) == 0)))
00334       break;
00335   if (!found_cal)
00336     return (vt_set_error("vt_read_calibration",CAL_ERROR1));
00337 
00338   /* read the calibration data */
00339   while (fgets(input_line,81,inputfp))
00340   {
00341     if (input_line[0] != '!')   /* skip comments */
00342     {
00343       while (sscanf(input_line,"FINGER %d:",&finger))
00344       {
00345     for (joint = 0; joint < JOINTS; joint++)
00346     {
00347       if (fgets(input_line,81,inputfp) == NULL)
00348       {
00349         return (vt_set_error("vt_read_calibration",CAL_ERROR1));
00350       }
00351 
00352       sscanf(input_line,"%d",&temp);
00353       if (temp == joint)
00354       {
00355         sscanf(input_line,"%*d%d%f", &offset,&gain);
00356             offset &= 0xFF;
00357             glove->private_data->mapping[finger][joint].offset = offset;
00358             glove->private_data->mapping[finger][joint].gain = gain;
00359       }
00360       else
00361       {
00362         return (vt_set_error("vt_read_calibration",CAL_ERROR1));
00363       }
00364     }
00365       }
00366     }
00367   }
00368 
00369   return(Ok);
00370 }
00371 
00372 void
00373 CyberGloveBasic::vt_write_glove_calibration(FILE *outputfp, CbGlove glove,
00374                char *version_string)
00375 {
00376   int finger,joint;
00377 
00378   fprintf(outputfp,"VHv%s\n",version_string);
00379   fprintf(outputfp,
00380       "! Virtex CbGlove (TM) -> Virtual Hand calibration file\n");
00381   fprintf(outputfp,"! == Virtual Hand Software %s ==\n!\n",version_string);
00382   fprintf(outputfp,"GLOVE CALIBRATION follows:\n");
00383   fprintf(outputfp,"! joint, offset, gain, gesture\n");
00384 
00385   for (finger=0; finger < FINGERS+1; finger++)
00386   {
00387     fprintf(outputfp,"FINGER %1d:\n",finger);
00388     for (joint = 0; joint < JOINTS; joint++)
00389       fprintf(outputfp,"%3d %6d %10.5f %5d\n", joint,
00390               glove->private_data->mapping[finger][joint].offset,
00391               glove->private_data->mapping[finger][joint].gain,
00392           0);
00393   }
00394 
00395   fprintf(outputfp,"! E_O_CAL\n!\n");
00396 }
00397 
00398 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00399 /*                                                               */
00400 /*  Routines to read and write both geometry and calibration     */
00401 /*  data.                                                        */
00402 /*                                                               */
00403 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00404 
00405 
00406 int
00407 CyberGloveBasic::vt_hand_read(VirtualHand hand, char *filename)
00408 {
00409   int success;
00410   FILE *inputfp;
00411   char errmessg[80] = "vt_hand_read\n     ";
00412 
00413   if ((inputfp = fopen(filename, "r")) == NULL)
00414   {
00415     strcat(errmessg,filename);
00416     return (vt_print_unix_error(errmessg,"fopen"));
00417   }
00418 
00419   success = vt_read_glove_calibration(inputfp,hand->private_data->glove);
00420   success = success && vt_geom_in(inputfp,hand->private_data->user);
00421   fclose(inputfp);
00422 
00423   hand->right_hand = &(hand->private_data->user->right_hand);
00424 
00425   /* build matrices for new geometry */
00426   vt_build_hand_matrices(hand);
00427 
00428   return (success);
00429 }
00430 
00431 int
00432 CyberGloveBasic::vt_hand_write(VirtualHand hand, char *filename, char *version_string)
00433 {
00434   FILE *outputfp;
00435   char errmessg[80] = "vt_hand_write\n     ";
00436 
00437   if ((outputfp = fopen(filename, "w")) == NULL)
00438   {
00439     strcat(errmessg,filename);
00440     return (vt_print_unix_error(errmessg,"fopen"));
00441   }
00442 
00443   vt_write_glove_calibration(outputfp, hand->private_data->glove,
00444                  version_string);
00445 
00446   vt_geom_out(outputfp,hand->private_data->user);
00447   fclose(outputfp);
00448   return (Ok);
00449 }
