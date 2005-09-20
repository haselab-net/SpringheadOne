--                                 */
00002 /*                                                              */
00003 /*  Virtual Hand initialization and control routines.           */
00004 /*                                                              */
00005 /*  The VirtualHand data structure encapsulates tracker and     */
00006 /*  glove data.                                                 */
00007 /*                                                              */
00008 /*  ==========================================================  */
00009 /*  -- Copyright (C) 1992,93 Virtual Technologies          --  */
00010 /*  --                                                      --  */
00011 /*  --    Author:  Larry Edwards                            --  */
00012 /*  ==========================================================  */
00013 
00014 #include <gadget/Devices/DriverConfig.h>
00015 #include <stdio.h>
00016 #include <stdlib.h>
00017 #include <math.h>
00018 
00019 #include <gadget/Devices/VirtualTechnologies/CyberGlove/CyberGlove.h>
00020 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_math3d.h>
00021 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_init.h>
00022 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_read_user_cal-types.h>
00023 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_virtual_hand-types.h>
00024 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_globals.h>
00025 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_error.h>
00026 
00027 typedef float Matrix[4][4];
00028 
00029 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00030 /*                                                             */
00031 /*  Initialization                                             */
00032 /*                                                             */
00033 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00034 
00035 
00036 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00037 /*  ALLOCATE_VIRTUALHAND --                                     */
00038 /*                                                              */
00039 /*  allocates and a VirtualHand structure and any subsidiary    */
00040 /*  structures                                                  */
00041 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00042 
00043 static VirtualHand
00044 allocate_VirtualHand(void)
00045 {
00046   VirtualHand newhand;
00047   VirtualHandPrivateStruct *newhand_private;
00048 
00049   if ((newhand = (VirtualHand) calloc(1,sizeof(VirtualHandStruct))) == NULL)
00050     vt_fatal_unix_error("allocate_VirtualHand","calloc");
00051 
00052   if ((newhand->private_data =
00053        (VirtualHandPrivateStruct *) calloc(1,sizeof(VirtualHandPrivateStruct))) == NULL)
00054     vt_fatal_unix_error("allocate_VirtualHand","calloc");
00055 
00056   newhand_private = newhand->private_data;
00057 
00058   if ((newhand_private->user =
00059        (UserGeometry) calloc(1,sizeof(UserGeometryStruct))) == NULL)
00060     vt_fatal_unix_error("allocate_VirtualHand","calloc");
00061 
00062   if ((newhand->surface =
00063        (SurfaceModel) calloc(1,sizeof(SurfaceModelStruct))) == NULL)
00064     vt_fatal_unix_error("allocate_VirtualHand","calloc");
00065 
00066   newhand->geom = newhand_private->user->geom;
00067 
00068   return (newhand);
00069 }
00070 
00071 
00072 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00073 /*  INIT_VIRTUALHAND --                                         */
00074 /*                                                              */
00075 /*  allocates and initializes a VirtualHand structure           */
00076 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00077 
00078 void
00079 CyberGloveBasic::init_VirtualHand(VirtualHand hand, int glove_index,
00080          char calfile[],
00081          Boolean visible)
00082 {
00083   VirtualHandPrivate hand_private = hand->private_data;
00084 
00085   hand_private->glove_index = glove_index;
00086 
00087   hand_private->glove = vt_create_CbGlove(FALSE);
00088   hand->timestamp = &(hand_private->glove->timestamp);
00089 
00090   hand->joint_angle = hand_private->glove->joint_angle;
00091   hand->raw_glove_value = hand_private->glove->raw_value;
00092 
00093   /* read in user's hand geometry and glove/tracker calibration */
00094   vt_hand_read(hand,calfile);
00095 
00096   vt_unprocess_glove_angles();
00097 }
00098 
00099 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00100 /*  VT_CREATE_VIRTUALHAND --                                    */
00101 /*                                                              */
00102 /*  creates a VirtualHand structure and reads in associated     */
00103 /*  configuration and data files                                */
00104 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00105 
00106 VirtualHand
00107 CyberGloveBasic::vt_create_VirtualHand( char *port, int baud, AppDataStruct &app )
00108 {
00109   VirtualHand thehand;
00110   int glove_index;
00111   Boolean right_hand_glove;
00112 
00113   thehand = allocate_VirtualHand();
00114 
00115   glove_index = 0;
00116   init_VirtualHand(thehand,glove_index,
00117            app.glove[glove_index].incalfile,
00118            app.hand_visible[glove_index]);
00119 
00120   thehand->read_glove = app.glove[glove_index].on;
00121   if (thehand->read_glove)
00122   {
00123     if (vt_open_glove_port(port,
00124                baud) < Ok)
00125     {
00126       vt_print_error("vt_create_VirtualHand");
00127       thehand->read_glove = FALSE;
00128     }
00129     else
00130     {
00131       vt_send_glove_query(CG_RIGHT_HAND_QUERY,&right_hand_glove);
00132       if (thehand->right_hand != NULL)
00133     if (right_hand_glove != *(thehand->right_hand))
00134       fprintf(stderr,"\nWARNING in vt_create_VirtualHand:\n\thandedness of"
00135           " calibration file does not match CbGlove.");
00136     }
00137   }
00138 
00139   return (thehand);
00140 }
00141 
00142 
00143 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00144 /*  VT_DESTROY_VIRTUALHAND --                                   */
00145 /*                                                              */
00146 /*  frees memory associated with a VirtualHand and closes any   */
00147 /*  associated open ports                                       */
00148 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00149 
00150 void
00151 CyberGloveBasic::vt_destroy_VirtualHand(VirtualHand hand)
00152 {
00153   vt_destroy_CbGlove(hand->private_data->glove);
00154   free(hand->private_data->user);
00155   free(hand->surface);
00156   free(hand->private_data);
00157   free(hand);
00158 }
00159 #if CJGLOVE == 1
00160 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00161 /*                                                             */
00162 /*  routines to get/set hand private info                      */
00163 /*                                                             */
00164 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
00165 
00166 
00167 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00168 /*  VT_HAND_USER_GEOM_ADDRESS --                                */
00169 /*                                                              */
00170 /*  Not for general use ... needed for vt_read_user_cal.c       */
00171 /*  and vt_graphics.c                                           */
00172 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00173 
00174 UserGeometry
00175 vt_hand_user_geom_address(VirtualHand hand)
00176 {
00177   VirtualHandPrivate private_data = hand->private_data;
00178 
00179   return(private_data->user);
00180 }
00181 
00182 
00183 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00184 /*  VT_HAND_GLOVE_ADDRESS --                                    */
00185 /*                                                              */
00186 /*  Not for general use ... needed for vt_panel.c               */
00187 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00188 
00189 CbGlove
00190 vt_hand_glove_address(VirtualHand hand)
00191 {
00192   VirtualHandPrivate private_data = hand->private_data;
00193 
00194   return(private_data->glove);
00195 }
00196 #endif
00197 
00198 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00199 /*  VT_BUILD_HAND_MATRICES --                                    */
00200 /*                                                               */
00201 /*  Generate the unflexed (i.e. all joint angles == 0) joint     */
00202 /*  transformations.                                             */
00203 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
00204 
00205 void
00206 CyberGloveBasic::vt_build_hand_matrices(VirtualHand hand)
00207 {
00208   VirtualHandPrivate private_data = hand->private_data;
00209   UserGeometry user = private_data->user;
00210   char finger, joint;
00211   vec3d digit,normal,thumb_mcp;
00212   static vec3d z_axis = {0.0,0.0,1.0};
00213   matrix4x4 rotmatrix;
00214   float oldtheta,theta,length;
00215 
00216   /* this function determines the transform for each joint */
00217   /* the transform that is stored with each joint is the transform */
00218   /* that gets you from the previous joints coord system to the local */
00219   /* coord system of the current joint */
00220 
00221   /* determine the transform for the first joint of each finger */
00222   for (finger = THUMB; finger < FINGERS; finger++)
00223   {
00224     joint = MCP;
00225     /* the following determines any skewness of the joint axis */
00226     if (finger != THUMB)
00227     {
00228       digit[VX] = user->geom[finger][joint+1][VX] -
00229                                     user->geom[finger][joint][VX];
00230       digit[VY] = user->geom[finger][joint+1][VY] -
00231                                         user->geom[finger][joint][VY];
00232       theta = atan2(digit[VY], digit[VX]) - M_PI/2.0;
00233     }
00234     else            /* for thumb we skew in x-y and y-z planes */
00235     {
00236       vt_normalize3(user->thumb_mcp_axis,thumb_mcp);
00237       if ((hand->right_hand != NULL) && *(hand->right_hand) != TRUE)
00238     vt_vec_neg3(thumb_mcp,thumb_mcp);
00239 
00240       /* determine the transformation to the unabducted MCP coordinate frame */
00241       vt_zero_matrix_fill(rotmatrix);
00242       rotmatrix[3][3] = 1.0;
00243       vt_normalize3(thumb_mcp,rotmatrix[0]); /* the thumb x-axis */
00244       vt_cross_prod3(z_axis,rotmatrix[0],normal); /* the thumb y-axis */
00245       vt_normalize3(normal,rotmatrix[1]);
00246       vt_cross_prod3(rotmatrix[0],rotmatrix[1],normal);
00247       vt_normalize3(normal,rotmatrix[2]); /* the z-axis */
00248 
00249       /* determine the unflexed abduction angle ... we can't include this */
00250       /* in the digit_xform because we must do the bend about the MCP axis */
00251       /* before we do the abduction*/
00252       vt_vec_sub3(user->geom[finger][joint+1],user->geom[finger][joint],digit);
00253       vt_normalize3(digit,digit);
00254       hand->unflexed_abduction = -(M_PI_2 - acos(vt_dot_prod3(digit,thumb_mcp)));
00255     }
00256     /* translate from the origin to the joint */
00257     vt_trans_matrix(user->geom[finger][joint],hand->digit_xform[finger][joint]);
00258     /* rotate by skew angle */
00259     if (finger != THUMB)
00260       vt_mult_rot_matrix(theta,'z',Premult,hand->digit_xform[finger][joint]);
00261     else
00262       vt_mult_matrix(rotmatrix,hand->digit_xform[finger][joint],
00263              hand->digit_xform[finger][joint]);
00264   }
00265 
00266   /* for thumb we just assume that the PIP and DIP axes are parallel since */
00267   /* we have no info to the contrary ... we just translate out along y-axis  */
00268   finger = THUMB;
00269   for (joint = PIP; joint < ABDUCT; joint++)
00270   {
00271     digit[VX] = user->geom[finger][joint][VX] - user->geom[finger][joint-1][VX];
00272     digit[VY] = user->geom[finger][joint][VY] - user->geom[finger][joint-1][VY];
00273     digit[VZ] = user->geom[finger][joint][VZ] - user->geom[finger][joint-1][VZ];
00274     length = sqrt(digit[VX]*digit[VX]+digit[VY]*digit[VY]+digit[VZ]*digit[VZ]);
00275 
00276     digit[VX] = 0.0;
00277     digit[VY] = length;
00278     digit[VZ] = 0.0;
00279     vt_trans_matrix(digit,hand->digit_xform[finger][joint]);
00280   }
00281 
00282   /* for fingers we translate out for each segment and then add a small */
00283   /* skew angle */
00284   for (finger = INDEX; finger < FINGERS; finger++)
00285   {
00286     joint = PIP;
00287     digit[VX] = user->geom[finger][joint][VX] - user->geom[finger][joint-1][VX];
00288     digit[VY] = user->geom[finger][joint][VY] - user->geom[finger][joint-1][VY];
00289     oldtheta = atan2(digit[VY],digit[VX]);
00290     for (; joint < ABDUCT; joint++)
00291     {
00292       digit[VX] = user->geom[finger][joint][VX] -
00293                                             user->geom[finger][joint-1][VX];
00294       digit[VY] = user->geom[finger][joint][VY] -
00295                                         user->geom[finger][joint-1][VY];
00296       digit[VZ] = user->geom[finger][joint][VZ] -
00297                                         user->geom[finger][joint-1][VZ];
00298       length = vt_vec_length3(digit);
00299       digit[VX] = 0.0;
00300       digit[VY] = length;
00301       digit[VZ] = 0.0;
00302       vt_trans_matrix(digit,hand->digit_xform[finger][joint]);
00303 
00304       digit[VX] = user->geom[finger][joint+1][VX] -
00305                                          user->geom[finger][joint][VX];
00306       digit[VY] = user->geom[finger][joint+1][VY] -
00307                                          user->geom[finger][joint][VY];
00308       theta = atan2(digit[VY],digit[VX]);
00309       vt_mult_rot_matrix(theta-oldtheta,'z',Premult,
00310              hand->digit_xform[finger][joint]);
00311       oldtheta = theta;
00312     }
00313   }
00314 }
00315 
00316 
00317 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00318 /*  VT_UPDATE_HAND_STATE --                                       */
00319 /*                                                                */
00320 /*  Update the hand's state data based on new data read or        */
00321 /*  changes in the slider panel                                   */
00322 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00323 
00324 void
00325 CyberGloveBasic::vt_update_hand_state(VirtualHand hand)
00326 {
00327  /* if hand is not on reflect changes due to sliders */
00328  if (hand->read_glove)
00329    vt_read_processed_glove_data();
00330  else
00331    vt_process_glove_data(); /* reflect changes due to sliders */
00332 }
00333 
00334 
00335 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00336 /*  VT_CALC_THUMB_ROLL --                                       */
00337 /*                                                              */
00338 /*  calculates the amount of thumb roll for a given thumb       */
00339 /*  position.                                                   */
00340 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
00341 
00342 float
00343 CyberGloveBasic::vt_calc_thumb_roll(VirtualHand hand)
00344 {
00345   float roll_vector[3],x_axis_desired[3],rot_axis[3];
00346   float *x_axis,*y_axis,*bone_vector;
00347   float roll_angle,cosangle;
00348   Matrix thumb_mcp_matrix;
00349   UserGeometry user = hand->private_data->user;
00350 
00351   vt_copy_matrix(hand->digit_xform[THUMB][MCP],thumb_mcp_matrix);
00352   vt_mult_rot_matrix(hand->joint_angle[THUMB][MCP],
00353              'x',Premult,thumb_mcp_matrix);
00354   vt_mult_rot_matrix(hand->joint_angle[THUMB][ABDUCT] +
00355              hand->unflexed_abduction,'z',Premult,thumb_mcp_matrix);
00356 
00357   /* the roll vector is the axis around which the thumb rolls as the thumb */
00358   /* abducts and pivots */
00359   vt_normalize3(user->thumb_roll_axis,roll_vector);
00360 
00361   x_axis = (float *) thumb_mcp_matrix[0];
00362   y_axis = (float *) thumb_mcp_matrix[1];
00363   bone_vector = y_axis;
00364 
00365   /* take cross product of vector along bone (current y-axis) and roll_vector */
00366   /* to find desired x-axis */
00367   vt_cross_prod3(roll_vector,bone_vector,x_axis_desired);
00368   vt_normalize3(x_axis_desired,x_axis_desired);
00369 
00370   /* take cross-prod of current MCP x-axis with desired x-axis */
00371   /* to find sense of required roll */
00372   vt_cross_prod3(x_axis,x_axis_desired,rot_axis);
00373 
00374   /* find angle between current MCP x-axis and desired x-axis */
00375   cosangle = vt_dot_prod3(x_axis,x_axis_desired);
00376   /* make sure absolute value of cosangle really is <= 1 */
00377   if (cosangle > 1.0)
00378     cosangle = 1.0;
00379   else if (cosangle < -1.0)
00380     cosangle = -1.0;
00381 
00382   roll_angle = acos(cosangle) * RAD2DEG;
00383 
00384   if (vt_dot_prod3(rot_axis,y_axis) < 0.0)
00385     roll_angle = -roll_angle;
00386 
00387   return (roll_angle);
00388 }
