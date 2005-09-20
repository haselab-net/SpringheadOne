 /*  FILE: vt_read_hand_model.c                                      */
00002 /*                                                                  */
00003 /*      routines for reading in and building boundary               */
00004 /*      representation of hand model                                */
00005 /*                                                                  */
00006 /*  ==============================================================  */
00007 /*  --  Copyright (C) 1992, 1993 Virtual Technologies           --  */
00008 /*  --                                                          --  */
00009 /*  --    Author: Larry Edwards                                 --  */
00010 /*  ==============================================================  */
00011 
00012 #include <gadget/Devices/DriverConfig.h>
00013 #include <stdio.h>
00014 #include <math.h>
00015 #include <stdlib.h>
00016 #include <X11/Intrinsic.h>
00017 
00018 #include <gadget/Devices/VirtualTechnologies/CyberGlove/CyberGlove.h>
00019 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_read_user_cal-types.h>
00020 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_virtual_hand-types.h>
00021 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_globals.h>
00022 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_math3d.h>
00023 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_read_object-types.h>
00024 
00025 
00026 static void
00027 transform_object(objptr thisobj, matrix4x4 xform)
00028 {
00029   int i,numverts;
00030   vertptr vertices;
00031 
00032   numverts = thisobj->numverts;
00033   vertices = thisobj->vertices;
00034 
00035   for (i=0; i < numverts; i++)
00036     vt_transform3(vertices[i].pos,xform,vertices[i].pos);
00037 }
00038 
00039 void
00040 read_model_geom(char infilename[], vec3d geom[5][4], vec3d thumb_roll_vecs[2])
00041 {
00042   char dummy[82], input_line[82];
00043   FILE *inputfp;
00044   int finger,joint,i;
00045   pos3d axis_seg[2];
00046   matrix4x4 rotmatrix;
00047 
00048   inputfp = fopen(infilename,"r");
00049 
00050   /* The nice gentlemen at Viewpoint have their hand pointing in */
00051   /* the -y direction */
00052   vt_rot_matrix(M_PI,'z',rotmatrix);
00053 
00054   for (i=0; i<2; i++)
00055   {
00056     while (sscanf(fgets(input_line, 81, inputfp), "%[#]", dummy));
00057     if (!feof(inputfp))
00058       sscanf(input_line,"%f %f %f",&axis_seg[0][VX],&axis_seg[0][VY],
00059          &axis_seg[0][VZ]);
00060     while (sscanf(fgets(input_line, 81, inputfp), "%[#]", dummy));
00061     if (!feof(inputfp))
00062       sscanf(input_line,"%f %f %f",&axis_seg[1][VX],&axis_seg[1][VY],
00063          &axis_seg[1][VZ]);
00064     /* inside edge minus outside edge of thumb */
00065     vt_vec_diff3(axis_seg[0],axis_seg[1],thumb_roll_vecs[i]);
00066     vt_transform3(thumb_roll_vecs[i],rotmatrix,thumb_roll_vecs[i]);
00067   }
00068 
00069   for (finger=THUMB; finger < FINGERS; finger++)
00070   {
00071     for (joint=MCP; joint < JOINTS; joint++)
00072     {
00073       /* skip comments */
00074       while (sscanf(fgets(input_line, 81, inputfp), "%[#]", dummy));
00075       if (feof(inputfp))
00076     break;
00077       sscanf(input_line,"%f %f %f",&geom[finger][joint][VX],
00078          &geom[finger][joint][VY],&geom[finger][joint][VZ]);
00079       vt_transform3(geom[finger][joint],rotmatrix,geom[finger][joint]);
00080     }
00081     if (feof(inputfp))
00082       break;
00083   }
00084   fclose(inputfp);
00085 }
00086 
00087 void
00088 angle_fudge_factors(int finger, int joint, vec3d angles)
00089 {
00090   angles[VX] = angles[VY] = angles[VZ] = 0.0;
00091 
00092   angles[VX] = 0.1;
00093   if (finger == THUMB)
00094     switch (joint)
00095     {
00096     case MCP: angles[VX] = -0.02; break;
00097     case PIP: angles[VX] = -0.1; angles[VZ] = 0.0; break;
00098     case DIP: break;
00099     }
00100 }
00101 
00102 void
00103 scale_fudge_factors(int finger, int joint, vec3d scalevec)
00104 {
00105   scalevec[VY] = 1.0;
00106   scalevec[VX] = scalevec[VZ] = 1.2; /* Make the finger diameter larger. */
00107 
00108   if (finger == THUMB)
00109     scalevec[VZ] = 1.0;
00110 }
00111 
00112 void
00113 translation_fudge_factors(int finger, int joint, vec3d transvec)
00114 {
00115   transvec[VX] = transvec[VY] = transvec[VZ] = 0.0;
00116 
00117   if (finger == THUMB && joint == DIP) {
00118       transvec[VX] = 0.0;
00119       transvec[VY] = -0.1;
00120       transvec[VZ] = 0.0;
00121     }
00122 }
00123 
00124 
00125 static void
00126 adjust_hand_model_geometry(vec3d geom[5][4], vec3d thumb_roll_vecs[2],
00127                VirtualHand hand)
00128 {
00129   UserGeometry user = hand->private_data->user;
00130   vec3d angles,scalevec,segvec,jointpos,handmodelpos,tempvec;
00131   matrix4x4 rotmatrix,transmatrix,scalematrix,totmatrix;
00132   float length_model_seg,length_hand_seg;
00133   int finger,joint;
00134 
00135   for (finger=THUMB; finger < FINGERS; finger++)
00136   {
00137     for (joint=MCP; joint < ABDUCT; joint++)
00138     {
00139       translation_fudge_factors(finger,joint,jointpos);
00140       angle_fudge_factors(finger,joint,angles);
00141       scale_fudge_factors(finger,joint,scalevec);
00142 
00143       /* first we move the joint to the origin */
00144       vt_vec_add3(jointpos,geom[finger][joint],jointpos);
00145       vt_vec_neg3(jointpos,jointpos);
00146       vt_trans_matrix(jointpos,transmatrix);
00147 
00148       /* then we rotate it */
00149       vt_vec_diff3(geom[finger][joint],geom[finger][joint+1],segvec);
00150 
00151       /* we align by rotating around z first then x */
00152       angles[VZ] += M_PI/2.0 - atan2(segvec[VY],segvec[VX]);
00153       vt_rot_matrix(angles[VZ],'z',rotmatrix);
00154       vt_transform3(segvec,rotmatrix,segvec);
00155       angles[VX] += -atan2(segvec[VZ],segvec[VY]);
00156       vt_mult_rot_matrix(angles[VX],'x',Postmult,rotmatrix);
00157       if ((finger == THUMB) && (joint != MCP)) /* we also roll */
00158       {
00159     vt_copy_vec3(thumb_roll_vecs[joint-1],tempvec);
00160     vt_transform3(thumb_roll_vecs[joint-1],rotmatrix,
00161               thumb_roll_vecs[joint-1]);
00162     /* after being transformed the roll vec should be perpendicular      */
00163     /* to the y-axis we assume roll vec is pointing to the inside of the */
00164     /* thumb. NOTE: below we do not negate the value returned by atan2   */
00165     /* this is because for the x-z plane the angle returned by atan2     */
00166     /* is the negative of the actual angle                               */
00167     angles[VY] += atan2(thumb_roll_vecs[joint-1][VZ],
00168                 thumb_roll_vecs[joint-1][VX]);
00169     vt_mult_rot_matrix(angles[VY],'y',Premult,rotmatrix);
00170       }
00171 
00172       /* finally we scale it */
00173       length_model_seg = vt_vec_length3(segvec);
00174       vt_vec_diff3(user->geom[finger][joint],user->geom[finger][joint+1],segvec);
00175       length_hand_seg = vt_vec_length3(segvec);
00176       scalevec[VY] *= length_hand_seg/length_model_seg;
00177       scalevec[VX] *= 1.8;
00178       scalevec[VZ] *= 1.8;
00179       if (user->right_hand != TRUE)
00180     scalevec[VX] = -scalevec[VX];
00181       vt_scale_matrix(scalevec,scalematrix);
00182 
00183       vt_mult_matrix(transmatrix,rotmatrix,totmatrix);
00184       vt_mult_matrix(totmatrix,scalematrix,totmatrix);
00185       transform_object(&(hand->surface->digit[finger][joint]),totmatrix);
00186     }
00187   }
00188 
00189   /* lets do the hand now... */
00190   vt_set_vec3(0.06,-4.21,0.0,handmodelpos);
00191   vt_vec_neg3(handmodelpos,handmodelpos);
00192   vt_trans_matrix(handmodelpos,transmatrix);
00193   /* determine scaling of hand model based on the location of the user's first */
00194   /* knuckle of the index finger relative to the origin */
00195   vt_set_vec3(0.06,-4.21,0.0,handmodelpos);
00196   vt_vec_diff3(handmodelpos,geom[INDEX][MCP],jointpos);
00197   if ((jointpos[VX] != 0.0) && (user->geom[INDEX][MCP][VX] != 0.0))
00198     scalevec[VX] = fabs(user->geom[INDEX][MCP][VX]/jointpos[VX]);
00199   else
00200     scalevec[VX] = 1.0;     /* can't get there by scaling! */
00201   if ((jointpos[VY] != 0.0) && (user->geom[INDEX][MCP][VY] != 0.0))
00202     scalevec[VY] = fabs(user->geom[INDEX][MCP][VY]/jointpos[VY]);
00203   else
00204     scalevec[VY] = 1.0;     /* can't get there by scaling! */
00205   scalevec[VX] *= 1.1;
00206   scalevec[VZ] = 1.9;
00207 
00208   if (user->right_hand != TRUE)
00209     scalevec[VX] = -scalevec[VX];
00210 
00211   vt_scale_matrix(scalevec,scalematrix);
00212   vt_mult_matrix(transmatrix,scalematrix,totmatrix);
00213   transform_object(&(hand->surface->palm),totmatrix);
00214 
00215   /* lets do the forearm now... translation and scaling of forearm are same */
00216   /* as hand, so we just reuse totmatrix */
00217   transform_object(&(hand->surface->forearm),totmatrix);
00218 }
00219 
00220 static void
00221 skip_comments(FILE *inputfp)
00222 {
00223   int ch;
00224   char comment_line[256];
00225 
00226   while (((ch = getc(inputfp)) == '!') && (ch != EOF))
00227     fgets(comment_line,256,inputfp);
00228   ungetc(ch,inputfp);
00229 }
00230 
00231 void
00232 CyberGloveBasic::vt_read_hand_model(char infilename[], VirtualHand hand, char *glovedir)
00233 {
00234   int finger,joint,numobjects;
00235   vec3d thumb_roll_vecs[2];
00236   vec3d geom[5][4];
00237   FILE *inputfp;
00238   matrix4x4 totmatrix,rotmatrix;
00239   static vec3d scalevec = {0.4475,0.4475,0.4475};
00240   static vec3d transvec = {-0.06,4.2,0.1};
00241   Boolean right_handed;
00242   char geomfile[100];
00243 
00244 /*  cout << "\nReading "<< infilename << " ..."; */
00245 
00246   if (hand->right_hand != NULL)
00247     right_handed = *(hand->right_hand);
00248   else
00249     right_handed = TRUE;
00250 
00251   inputfp = fopen(infilename,"r");
00252   /* skip any comments at the beginning of the file */
00253   skip_comments(inputfp);
00254   fscanf(inputfp,"\tTotal Number of Objects In World:%d\n",&numobjects);
00255 
00256   if (numobjects < (1+FINGERS*ABDUCT))
00257     printf("WARNING in vt_read_hand_model: file contains too few objects");
00258 
00259   /* the palm and first phalanx of the thumb were created at a different     */
00260   /* scale and orientation than the rest of the model, so we use a different */
00261   /* transform for them                                                      */
00262   vt_rot_matrix(M_PI,'z',totmatrix);
00263   vt_mult_scale_matrix(scalevec,Postmult,totmatrix);
00264   vt_mult_trans_matrix(transvec,Postmult,totmatrix);
00265   vt_mult_rot_matrix(M_PI,'z',Postmult,totmatrix);
00266 
00267   /* Viewpoint has their hand pointing in the -y direction !@%$&*^ */
00268   vt_rot_matrix(M_PI,'z',rotmatrix);
00269 
00270   vt_read_object(inputfp,&(hand->surface->forearm));
00271   transform_object(&(hand->surface->forearm),rotmatrix);
00272 
00273   vt_read_object(inputfp,&(hand->surface->palm));
00274   transform_object(&(hand->surface->palm),totmatrix);
00275 
00276   for (finger=THUMB; finger < FINGERS; finger++)
00277     for (joint=MCP; joint < ABDUCT; joint++)
00278     {
00279       vt_read_object(inputfp,&(hand->surface->digit[finger][joint]));
00280       if ((finger == THUMB) && (joint == MCP))
00281     transform_object(&(hand->surface->digit[finger][joint]),totmatrix);
00282       else
00283     transform_object(&(hand->surface->digit[finger][joint]),rotmatrix);
00284 
00285       vt_calculate_face_normals(&(hand->surface->digit[finger][joint]),
00286                 right_handed);
00287       vt_calculate_dihedral_angles(&(hand->surface->digit[finger][joint]));
00288       vt_calculate_vertex_normals(&(hand->surface->digit[finger][joint]));
00289     }
00290   /* HACK ALERT hardwired filename - LJE */
00291   sprintf(geomfile, "%s/%s", glovedir, "hand_model.geom");
00292   read_model_geom(geomfile,geom,thumb_roll_vecs);
00293   adjust_hand_model_geometry(geom,thumb_roll_vecs,hand);
00294 
00295   vt_calculate_face_normals(&(hand->surface->forearm),right_handed);
00296   vt_calculate_dihedral_angles(&(hand->surface->forearm));
00297   vt_calculate_vertex_normals(&(hand->surface->forearm));
00298 
00299   vt_calculate_face_normals(&(hand->surface->palm),right_handed);
00300   vt_calculate_dihedral_angles(&(hand->surface->palm));
00301   vt_calculate_vertex_normals(&(hand->surface->palm));
00302 
00303   for (finger=THUMB; finger < FINGERS; finger++)
00304     for (joint=MCP; joint < ABDUCT; joint++)
00305     {
00306       vt_calculate_face_normals(&(hand->surface->digit[finger][joint]),
00307                 right_handed);
00308       vt_calculate_dihedral_angles(&(hand->surface->digit[finger][joint]));
00309       vt_calculate_vertex_normals(&(hand->surface->digit[finger][joint]));
00310     }
00311 
00312   fclose(inputfp);
00313 }
00314 
00315 void
00316 CyberGloveBasic::vt_read_lowres_hand_model(char infilename[], VirtualHand hand)
00317 {
00318   int finger,joint,numobjects;
00319   FILE *inputfp;
00320   matrix4x4 totmatrix;
00321   static vec3d digitvec,scalevec;
00322   Boolean right_handed;
00323 
00324   if (hand->right_hand != NULL)
00325     right_handed = *(hand->right_hand);
00326   else
00327     right_handed = TRUE;
00328 
00329   inputfp = fopen(infilename,"r");
00330   /* skip any comments at the beginning of the file */
00331   skip_comments(inputfp);
00332   fscanf(inputfp,"\tTotal Number of Objects In World:%d\n",&numobjects);
00333 
00334   if (numobjects < 3)
00335     printf("WARNING in vt_read_hand_model: file contains too few objects");
00336 
00337   vt_read_object(inputfp,&(hand->surface->forearm));
00338   vt_set_vec3(3.5,1.0,1.5,scalevec);
00339   if (!right_handed)
00340     scalevec[VX] = -scalevec[VX];
00341 
00342   vt_scale_matrix(scalevec,totmatrix);
00343   transform_object(&(hand->surface->forearm),totmatrix);
00344   vt_calculate_face_normals(&(hand->surface->forearm),right_handed);
00345   vt_calculate_dihedral_angles(&(hand->surface->forearm));
00346   vt_calculate_vertex_normals(&(hand->surface->forearm));
00347 
00348   vt_read_object(inputfp,&(hand->surface->palm));
00349   vt_set_vec3(1.0,1.0,1.0,scalevec);
00350   if (!right_handed)
00351     scalevec[VX] = -scalevec[VX];
00352 
00353   vt_scale_matrix(scalevec,totmatrix);
00354   transform_object(&(hand->surface->palm),totmatrix);
00355   vt_calculate_face_normals(&(hand->surface->palm),right_handed);
00356   vt_calculate_dihedral_angles(&(hand->surface->palm));
00357   vt_calculate_vertex_normals(&(hand->surface->palm));
00358 
00359   for (finger=THUMB; finger < FINGERS; finger++)
00360     for (joint=MCP; joint < ABDUCT; joint++)
00361     {
00362       vt_read_object(inputfp,&(hand->surface->digit[finger][joint]));
00363       vt_vec_sub3(hand->geom[finger][joint],hand->geom[finger][joint+1],
00364           digitvec);
00365       vt_set_vec3(1.0,vt_vec_length3(digitvec)/5.0,1.0,scalevec);
00366 
00367       if (!right_handed)
00368     scalevec[VX] = -scalevec[VX];
00369 
00370       vt_scale_matrix(scalevec,totmatrix);
00371       transform_object(&(hand->surface->digit[finger][joint]),totmatrix);
00372       vt_calculate_face_normals(&(hand->surface->digit[finger][joint]),
00373                 right_handed);
00374       vt_calculate_dihedral_angles(&(hand->surface->digit[finger][joint]));
00375       vt_calculate_vertex_normals(&(hand->surface->digit[finger][joint]));
00376     }
00377 
00378   fclose(inputfp);
00379 }
