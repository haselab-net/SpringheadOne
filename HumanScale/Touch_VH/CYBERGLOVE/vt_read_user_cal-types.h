/*  FILE: vt_read_user_cal-types.h                              */
00002 /*                                                              */
00003 /*  Type declarations for vt_read_user_cal.c                    */
00004 /*                                                              */
00005 /*  ==========================================================  */
00006 /*  --  Copyright (C) Virtual Technologies 1990,91,92,93    --  */
00007 /*  --                                                      --  */
00008 /*  --    Author:  William L. Chapin                        --  */
00009 /*  ==========================================================  */
00010 
00011 #ifndef VT_READ_USER_CAL_TYPES_H
00012 #define VT_READ_USER_CAL_TYPES_H
00013 
00014 #include <gadget/Devices/DriverConfig.h>
00015 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_math3d.h>
00016 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_glove-types.h>
00017 
00018 typedef struct
00019 {
00020   int fingers,joints;
00021   Boolean right_hand;
00022   vec3d geom[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES];
00023   vec3d pisiform;
00024   float knuckle_radius[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES];
00025   vec3d radi_ulna[8], forearm[8];
00026   vec3d thumb_roll_axis;
00027   vec3d thumb_mcp_axis;
00028   float thumb_roll;
00029   float hand_roll;
00030   vec3d wrist_offset;       /* offset of wrist from tracker receiver */
00031 } UserGeometryStruct,*UserGeometry;
00032 
00033 #endif /* VT_READ_USER_CAL_TYPES_H */
