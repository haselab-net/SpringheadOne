00001 /*  FILE: vt_read_object.c                                          */
00002 /*                                                                  */
00003 /*      routines for reading in and building 3D object boundary     */
00004 /*      representation                                              */
00005 /*                                                                  */
00006 /*  ==============================================================  */
00007 /*  --  Copyright (C) 1992, 1993 Virtual Technologies           --  */
00008 /*  --  Copyright (C) 1987,88,89,90,91,92 Larry Edwards         --  */
00009 /*  --                                                          --  */
00010 /*  --    Author: Larry Edwards                                 --  */
00011 /*  ==============================================================  */
00012 
00013 #include <gadget/Devices/DriverConfig.h>
00014 #include <stdio.h>
00015 #include <stdlib.h>
00016 #include <math.h>
00017 #include <sys/types.h>
00018 
00019 #include <X11/Intrinsic.h>
00020 
00021 #include <gadget/Devices/VirtualTechnologies/CyberGlove/CyberGlove.h>
00022 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_globals.h>
00023 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_math3d.h>
00024 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_error.h>
00025 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_read_object-types.h>
00026 
00027 int
00028 vt_prev_index(int current_index, int numelems)
00029 {
00030   return ((--current_index + numelems) % numelems);
00031 }
00032 
00033 int
00034 vt_next_index(int current_index, int numelems)
00035 {
00036   return (++current_index % numelems);
00037 }
00038 
00039 int
00040 vt_get_index(void *baseofarray, void *thiselem, int sizeof_elem)
00041 {
00042   caddr_t base,elem;
00043 
00044   base = (caddr_t) baseofarray;
00045   elem = (caddr_t) thiselem;
00046 
00047   return ((elem-base)/sizeof_elem);
00048 }
00049 
00050 loopsegptr
00051 CyberGloveBasic::vt_loopseg_twin(loopsegptr theloopseg)
00052 {
00053   if (theloopseg->edge == NULL)
00054     return(theloopseg);
00055 
00056   if (theloopseg->edge->loopseg1 == theloopseg)
00057     return(theloopseg->edge->loopseg2);
00058 
00059   return(theloopseg->edge->loopseg1);
00060 }
00061 
00062 static void
00063 read_matrix(FILE *infile, matrix4x4 thematrix)
00064 {
00065   int i,j;
00066   ;
00067   for (i = 0; i < 4; ++i)
00068   {
00069     fscanf(infile," [ ");
00070     for (j = 0; j <4; ++j)
00071       fscanf(infile," %f ",&(thematrix[i][j]));
00072     fscanf(infile," ]\n");
00073   }
00074 }
00075 
00076 static void
00077 read_vertices(FILE *infile, objptr thisobj)
00078 {
00079   int i,vertex_index,numverts;
00080   vec3d newvert;
00081 
00082   fscanf (infile," Number of Vertices:%d\n",&numverts);
00083   fscanf (infile," VFE List:\n");
00084 
00085   /* we allocate one more than numverts in case indices are 1 based */
00086   /* instead of 0 based */
00087   if ((thisobj->vertices = (vertptr) calloc(numverts+1,sizeof(vertstruct))) ==
00088       NULL)
00089     vt_fatal_unix_error("read_vertices","vertstruct calloc");
00090 
00091   thisobj->numverts = numverts;
00092 
00093   for (i = 0; i < numverts; ++i)
00094   {
00095     fscanf (infile,"Vertex %d:%f %f %f\n",&vertex_index,
00096         &(newvert[VX]),&(newvert[VY]),&(newvert[VZ]));
00097     if (vertex_index < numverts)
00098     {
00099       thisobj->vertices[vertex_index].pos[VX] = newvert[VX];
00100       thisobj->vertices[vertex_index].pos[VY] = newvert[VY];
00101       thisobj->vertices[vertex_index].pos[VZ] = newvert[VZ];
00102       thisobj->vertices[vertex_index].pos[VW] = 1.0;
00103     }
00104     else
00105       printf("WARNING in read_vertices: index is >= numverts\n");
00106   }
00107 }
00108 
00109 static void
00110 read_face_loop(FILE *infile, objptr thisobj, faceptr thisface)
00111 {
00112   int dumfacenum,dumnumbounds,dumboundnum,index,numloopsegs,vertex_index;
00113   int numverts;
00114   ;
00115   /* we ignore the face index and boundary count */
00116   fscanf(infile,"Face:%d (%d bounds)\n",&dumfacenum,&dumnumbounds);
00117   /* le's are loopsegs */
00118   fscanf (infile,"\tBound %d (%d le's):",&dumboundnum,&numloopsegs);
00119 
00120   if ((thisface->loopsegs =
00121        (loopsegptr) calloc(numloopsegs,sizeof(loopsegstruct))) == NULL)
00122     vt_fatal_unix_error("read_face_loop","loopsegstruct calloc");
00123 
00124   thisface->numloopsegs = numloopsegs;
00125 
00126   numverts = thisobj->numverts;
00127 
00128   for (index = 0; index < numloopsegs; ++index)
00129   {
00130     fscanf (infile,"%d ",&vertex_index);
00131     if (vertex_index < numverts)
00132     {
00133       thisface->loopsegs[index].parentface = thisface;
00134       thisface->loopsegs[index].vert = &(thisobj->vertices[vertex_index]);
00135       thisface->loopsegs[index].vert->loopseg0 = &(thisface->loopsegs[index]);
00136       thisface->loopsegs[index].edge = NULL; /* determined in make_edges */
00137     }
00138     else
00139       printf("WARNING in read_face_loop: index is >= numverts\n");
00140   }
00141 }
00142 
00143 static void
00144 read_faces(FILE *infile, objptr thisobj)
00145 {
00146   int index,numfaces;
00147 
00148   fscanf (infile," Number of Faces:%d\n",&numfaces);
00149   fscanf(infile," FVE List:\n");
00150 
00151   if ((thisobj->faces = (faceptr) calloc(numfaces,sizeof(facestruct))) == NULL)
00152     vt_fatal_unix_error("read_faces","facestruct calloc");
00153 
00154   thisobj->numfaces = numfaces;
00155 
00156   /* we ignore the face index in the file and just make it base 0 */
00157   for (index = 0; index < numfaces; ++index)
00158   {
00159     thisobj->faces[index].parentobj = thisobj;
00160     read_face_loop(infile,thisobj,&(thisobj->faces[index]));
00161   }
00162 }
00163 
00164 static loopsegptr
00165 find_loopseg(objptr thisobj, faceptr thisface, vertptr v1, vertptr v2)
00166 {
00167   loopsegptr next_loopseg,loopsegs;
00168   int i,numloopsegs;
00169   ;
00170   loopsegs = thisface->loopsegs;
00171   numloopsegs = thisface->numloopsegs;
00172 
00173   for (i=0; i < numloopsegs; i++)   /* go through loop forwards */
00174   {
00175     next_loopseg = &(loopsegs[vt_next_index(i,numloopsegs)]);
00176     if (((loopsegs[i].vert == v1) && (next_loopseg->vert == v2)) ||
00177     ((loopsegs[i].vert == v2) && (next_loopseg->vert == v1)))
00178       return (&loopsegs[i]);
00179   }
00180 
00181   return (NULL);        /*  the edge was not in this face */
00182 }
00183 
00184 static loopsegptr
00185 find_other_loopseg(objptr thisobj, faceptr currentface, int loopsegindex)
00186 {
00187   loopsegptr loopsegs,other_loopseg,next_loopseg;
00188   int i,numloopsegs;
00189   ;
00190   loopsegs = currentface->loopsegs;
00191   numloopsegs = currentface->numloopsegs;
00192   next_loopseg = &(loopsegs[vt_next_index(loopsegindex,numloopsegs)]);
00193   for (i=0; i<thisobj->numfaces; i++)
00194   {
00195     if (&(thisobj->faces[i]) != currentface)
00196     {
00197       other_loopseg = find_loopseg(thisobj,&(thisobj->faces[i]),
00198                    loopsegs[loopsegindex].vert,
00199                    next_loopseg->vert);
00200       if ((other_loopseg != NULL) && (other_loopseg->edge == NULL))
00201     return (other_loopseg);
00202     }
00203   }
00204 
00205   /* this should never happen!!! */
00206   /* vt_print_error("find_other_loopseg: other loopseg not found!\n");*/
00207   return (NULL);
00208 }
00209 
00210 static void
00211 make_edges(objptr thisobj)
00212 {
00213   faceptr faces;
00214   loopsegptr other_loopseg,loopsegs;
00215   edgeptr edges;
00216   int i,j,k,numedges,numloopsegs, edge_index = 0;
00217 
00218   faces = thisobj->faces;
00219 
00220   numedges = thisobj->numfaces + thisobj->numverts - 2;
00221   if ((thisobj->edges = (edgeptr) calloc(numedges,sizeof(edgestruct))) == NULL)
00222     vt_fatal_unix_error("make_edges","edgestruct calloc");
00223 
00224   edges = thisobj->edges;
00225 
00226   for (i=0; i < thisobj->numfaces; i++)
00227   {
00228     numloopsegs = faces[i].numloopsegs;
00229     loopsegs = faces[i].loopsegs;
00230     for (j=0; j < numloopsegs; j++)
00231     {
00232       if (loopsegs[j].edge == NULL)
00233       {
00234     /* the following should never happen but we may have a topologically */
00235     /* bad object so we check anyway */
00236     if (edge_index >= numedges)
00237     {
00238 /* Put back someday when we have valid objects
00239       printf("\nWARNING in make_edges: F+V-E != 2, continuing anyway");
00240 */
00241       /* make new bigger edges array */
00242       edges = thisobj->edges = (edgeptr) realloc(edges,(numedges+64)*
00243                              sizeof(edgestruct));
00244       /* point loopseg ptrs at the new edges array elements */
00245       for (k=0; k<numedges; k++)
00246       {
00247         if (edges[k].loopseg1 != NULL) /* check just in case... */
00248           edges[k].loopseg1->edge = &(edges[k]);
00249         if (edges[k].loopseg2 != NULL)
00250           edges[k].loopseg2->edge = &(edges[k]);
00251       }
00252       numedges += 64;
00253     }
00254 
00255     other_loopseg = find_other_loopseg(thisobj,&faces[i],j);
00256     if (other_loopseg == NULL)
00257       other_loopseg = &(loopsegs[j]);
00258 
00259 
00260     /* a loopseg always points to the edge having endpoints of */
00261     /* loopseg->vert and loopseg[next_index(i,numloopsegs)].vert */
00262     loopsegs[j].edge = &(edges[edge_index]);
00263     other_loopseg->edge = &(edges[edge_index]);
00264     edges[edge_index].loopseg1 = &(loopsegs[j]);
00265     edges[edge_index].loopseg2 = other_loopseg;
00266     edges[edge_index].vertex1 = loopsegs[j].vert;
00267     edges[edge_index].vertex2 = other_loopseg->vert;
00268 
00269     edge_index++;
00270       }
00271     }
00272   }
00273   thisobj->numedges = edge_index;
00274 }
00275 
00276 void
00277 CyberGloveBasic::vt_calculate_face_normals(objptr thisobj, Boolean clockwise)
00278 {
00279   /* NOTE: we assume a consistent face orientation here! */
00280   int i,j,nextj,numloopsegs;
00281   float *norm;
00282   faceptr faces;
00283   loopsegptr loopsegs;
00284 
00285   faces = thisobj->faces;
00286 
00287   for (i=0; i< thisobj->numfaces; i++)
00288   {
00289     loopsegs = faces[i].loopsegs;
00290     numloopsegs = faces[i].numloopsegs;
00291     norm = faces[i].norm;
00292     norm[VX] = norm[VY] = norm[VZ] = 0.0;
00293     if (clockwise)
00294     {
00295       for (j=0; j < numloopsegs; j++)
00296       {
00297     nextj = vt_next_index(j,numloopsegs);
00298 
00299     norm[VX] += (loopsegs[j].vert->pos[VY] - loopsegs[nextj].vert->pos[VY]) *
00300       (loopsegs[j].vert->pos[VZ] + loopsegs[nextj].vert->pos[VZ]);
00301 
00302     norm[VY] += (loopsegs[j].vert->pos[VZ] - loopsegs[nextj].vert->pos[VZ]) *
00303       (loopsegs[j].vert->pos[VX] + loopsegs[nextj].vert->pos[VX]);
00304 
00305     norm[VZ] += (loopsegs[j].vert->pos[VX] - loopsegs[nextj].vert->pos[VX]) *
00306       (loopsegs[j].vert->pos[VY] + loopsegs[nextj].vert->pos[VY]);
00307       }
00308     }
00309     else
00310     {
00311       for (j=0; j < numloopsegs; j++)
00312       {
00313     nextj = vt_next_index(j,numloopsegs);
00314 
00315     norm[VX] -= (loopsegs[j].vert->pos[VY] - loopsegs[nextj].vert->pos[VY]) *
00316       (loopsegs[j].vert->pos[VZ] + loopsegs[nextj].vert->pos[VZ]);
00317 
00318     norm[VY] -= (loopsegs[j].vert->pos[VZ] - loopsegs[nextj].vert->pos[VZ]) *
00319       (loopsegs[j].vert->pos[VX] + loopsegs[nextj].vert->pos[VX]);
00320 
00321     norm[VZ] -= (loopsegs[j].vert->pos[VX] - loopsegs[nextj].vert->pos[VX]) *
00322       (loopsegs[j].vert->pos[VY] + loopsegs[nextj].vert->pos[VY]);
00323       }
00324     }
00325     vt_normalize3(norm,norm);
00326   }
00327 
00328 }
00329 
00330 void
00331 CyberGloveBasic::vt_calculate_dihedral_angles(objptr thisobj)
00332 {
00333   int i;
00334   edgeptr edges;
00335   float *norm1,*norm2;
00336 
00337   edges = thisobj->edges;
00338   for (i=0; i< thisobj->numedges; i++)
00339   {
00340     norm1 = edges[i].loopseg1->parentface->norm;
00341     norm2 = edges[i].loopseg2->parentface->norm;
00342     /* for the moment we don't care wether angle is acute or obtuse */
00343     edges[i].dihedral_angle = RAD2DEG * acos(vt_dot_prod3(norm1,norm2));
00344   }
00345 }
00346 
00347 void
00348 CyberGloveBasic::vt_calculate_vertex_normals(objptr thisobj)
00349 {
00350   /* NOTE: we assume a consistent clockwise face orientation here! */
00351   int i,numfaces,loopsegindex;
00352   float *vertnorm;
00353   vertptr vertices;
00354   loopsegptr loopseg0,thisloopseg,nextloopseg;
00355   faceptr parentface;
00356 
00357   vertices = thisobj->vertices;
00358 
00359   for (i=0; i< thisobj->numverts; i++)
00360   {
00361     numfaces = 0;
00362     vertnorm = vertices[i].norm;
00363     vertnorm[VX] = vertnorm[VY] = vertnorm[VZ] = 0.0;
00364     thisloopseg = loopseg0 = vertices[i].loopseg0;
00365     if (thisloopseg != NULL)
00366       do
00367       {
00368     vertnorm[VX] += thisloopseg->parentface->norm[VX];
00369     vertnorm[VY] += thisloopseg->parentface->norm[VY];
00370     vertnorm[VZ] += thisloopseg->parentface->norm[VZ];
00371     numfaces++;
00372 
00373     nextloopseg = vt_loopseg_twin(thisloopseg);
00374     if (thisloopseg == nextloopseg)
00375       break;
00376 
00377     thisloopseg = nextloopseg;
00378     parentface = thisloopseg->parentface;
00379     loopsegindex = vt_get_index(parentface->loopsegs,thisloopseg,
00380                     sizeof(loopsegstruct));
00381     thisloopseg = &(parentface->loopsegs[vt_next_index(loopsegindex,
00382                    parentface->numloopsegs)]);
00383       } while (thisloopseg != loopseg0);
00384 
00385     vertnorm[VX] = vertnorm[VX] / (float) numfaces;
00386     vertnorm[VY] = vertnorm[VY] / (float) numfaces;
00387     vertnorm[VZ] = vertnorm[VZ] / (float) numfaces;
00388     vt_normalize3(vertnorm,vertnorm);
00389   }
00390 }
00391 
00392 void
00393 CyberGloveBasic::vt_read_object(FILE *infile, objptr thisobj)
00394 {
00395   int ch,dumobjnum,dumnumshells,dumshellnum;
00396   char objname[256],comment_line[256];
00397 
00398   /* skip any comments before each object */
00399   while (((ch = getc(infile)) == '!') && (ch != EOF))
00400     fgets(comment_line,256,infile);
00401   ungetc(ch,infile);
00402 
00403   /* the following 2 lines are only in the file format for readability */
00404   fscanf(infile,"\n\n\tObject %d:\n",&dumobjnum);
00405   fscanf(infile,"Object Name:%s\n",objname);
00406 
00407   fscanf (infile," Object Transform:\n");
00408   read_matrix(infile,thisobj->xform);
00409   fscanf (infile," Object Inverse Transform:\n");
00410   read_matrix(infile,thisobj->invxform);
00411 
00412   /* the following 2 lines of information are not used although */
00413   /* they may be in the future */
00414   fscanf (infile,"\nNumber of Shells:%d\n",&dumnumshells);
00415   fscanf(infile, "Shell %d:\n", &dumshellnum);
00416 
00417   /* read in all the vertices */
00418   read_vertices(infile,thisobj);
00419   /* read in all the faces */
00420   read_faces(infile,thisobj);
00421   /* construct the edges from the loopedges */
00422   make_edges(thisobj);
00423 }

