 /*************** <auto-copyright.pl BEGIN do not edit this line> **************
00002  *
00003  * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
00004  *
00005  * Original Authors:
00006  *   Allen Bierbaum, Christopher Just,
00007  *   Patrick Hartling, Kevin Meinert,
00008  *   Carolina Cruz-Neira, Albert Baker
00009  *
00010  * This library is free software; you can redistribute it and/or
00011  * modify it under the terms of the GNU Library General Public
00012  * License as published by the Free Software Foundation; either
00013  * version 2 of the License, or (at your option) any later version.
00014  *
00015  * This library is distributed in the hope that it will be useful,
00016  * but WITHOUT ANY WARRANTY; without even the implied warranty of
00017  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
00018  * Library General Public License for more details.
00019  *
00020  * You should have received a copy of the GNU Library General Public
00021  * License along with this library; if not, write to the
00022  * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
00023  * Boston, MA 02111-1307, USA.
00024  *
00025  * -----------------------------------------------------------------
00026  * File:          CyberGloveBasic.h,v
00027  * Date modified: 2003/03/05 16:54:52
00028  * Version:       1.21
00029  * -----------------------------------------------------------------
00030  *
00031  *************** <auto-copyright.pl END do not edit this line> ***************/
00032 
00033 #ifndef _CYBER_GLOVE_BASIC_H_
00034 #define _CYBER_GLOVE_BASIC_H_
00035 
00036 #include <gadget/Devices/DriverConfig.h>
00037 
00038 #include <stdio.h>
00039 #include <vpr/vpr.h>
00040 
00041 #ifdef VPR_OS_HPUX
00042 #   include <float.h>
00043 #   include <limits>
00044 #endif
00045 
00046 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_types.h>
00047 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_glove-types.h>
00048 #include <gadget/Devices/VirtualTechnologies/CyberGlove/vt_virtual_hand-types.h>
00049 
00050 struct CYBER_GLOVE_DATA {
00051   float joints[MAX_SENSOR_GROUPS][MAX_SENSOR_VALUES];
00052  // matrix4x4 xforms[5][3];
00053 };
00054 
00055 /* a note about fingers...
00056  * the GetJoints and GetDigitXForms both return 2d arrays.  The
00057  * first index of each of these is a finger, enumerated in vt_glove-types.h
00058  *    THUMB  = 0
00059  *    INDEX  = 1
00060  *    MIDDLE = 2
00061  *    RING   = 3
00062  *    PINKY  = 4
00063  *    CARPUS = 5    (refers to the wrist joints)
00064  * note that the joints array is 6 items, the digit xforms are 5 items.
00065  * The Fingers defined above can also be used as the first argument to
00066  * GetFingerState.
00067  * Each finger has 4 positions, which are the 2nd index to the array:
00068  *    MCP    = 0    (first knuckle)
00069  *    PIP    = 1
00070  *    DIP    = 2    (2nd knuckle - closest to fingertip
00071  *    ABDUCT = 3    (lateral spread of each finger)
00072  * The CARPUS is a little different - you have WRIST_PITCH and WRIST_YAW
00073  * to work with instead of finger knuckles.
00074  *
00075  * Note to those who like pointer math:  returning 2d arrays is troublesome,
00076  * so the GetJoints and GetDigitXForms actually return float** / matrix4x4**
00077  * -- that is, pointers to an array of pointers to arrays of objects.
00078  * But all you have to do is look at the results like this:
00079  *    float ** joints = glove.GetJoints();
00080  *    cout << "this joint is the middle finger, 3rd knuckle:" <<
00081  *         joints[MIDDLE][DIP];
00082  */
00083 class CyberGloveBasic
00084 {
00085 public:
00086 
00100    CyberGloveBasic(char* calFileDir, char *serport, int baud);
00101 
00103    ~CyberGloveBasic ();
00104 
00109    int open();
00110 
00111    int close();
00112 
00113    int sample();
00114 
00115    CYBER_GLOVE_DATA* getData();
00116 
00122    matrix4x4** getDigitXForms();
00123 
00125    float getThumbRoll();
00126 
00135    void readSurfaceModel(int resolution, char *dir, char *name);
00136 
00142    SurfaceModel getSurfaceModel();
00143 
00154    volatile float *getStateVec();
00155 
00160    float getThumbUnflexedAbduction();
00161 
00168    int getFingerState(int finger);
00169 
00170 private:
00171    CYBER_GLOVE_DATA theData;    //[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES];
00172 
00173    /* we're gonna start out with all the old glove stuff still global...*/
00174    VirtualHand hand;
00175    CbGlove current_glove;
00176    CbGlovePrivate current_glove_private;
00177    volatile float *joints[MAX_SENSOR_GROUPS];
00178    matrix4x4 *xforms[5];
00179 
00180    int     needToClosePorts;
00181 
00182    // Config params
00183    // XXX: We should allow the user to specify a config file
00184    char*   mCalFileDir;         
00185    char*   mSerialPort;         
00186    int     mBaudRate;           
00188    bool    mItsOpen;            
00190 private:
00191    /* private member functions */
00196 
00197    // vt_init.cpp
00198     void vt_init(char *dir, AppDataStruct &app);
00199 
00200     // vt_virtual_hand.cpp
00201     VirtualHand vt_create_VirtualHand(char *port, int baud,
00202                      AppDataStruct &app );
00203     void vt_destroy_VirtualHand(VirtualHand hand);
00204     void *vt_hand_receiver_map_address(VirtualHand hand);
00205     VirtualHand vt_virtual_hand_from_glove_id(int glove_id);
00206     void vt_build_hand_matrices(VirtualHand hand);
00207     void vt_build_flexed_hand_matrices(VirtualHand hand,
00208                       matrix4x4 palm_matrix,
00209                    matrix4x4 finger_matrix[][MAX_SENSOR_GROUPS]);
00210     void vt_build_hand_pos_orient_matrix(VirtualHand hand,
00211                         matrix4x4 hand_matrix);
00212     void vt_request_new_hand_data(VirtualHand hand);
00213     void vt_update_hand_state(VirtualHand hand);
00214     float vt_calc_thumb_roll(VirtualHand hand);
00215     void init_VirtualHand(VirtualHand hand, int glove_index,
00216          char calfile[],
00217          Boolean visible);
00218 
00219     // vt_read_user_cal.cpp
00220     int vt_geom_in(FILE *inputfp, UserGeometry user);
00221     void vt_geom_out( FILE *outputfp, UserGeometry user);
00222     int vt_read_glove_calibration(FILE *infile, CbGlove glove);
00223     void vt_write_glove_calibration(FILE *outputfp, CbGlove glove,
00224                        char *version_string);
00225     int vt_hand_read(VirtualHand hand, char *filename);
00226     int vt_hand_write(VirtualHand hand, char *filename, char *version_string);
00227 
00228     // vt_glove.h
00229     int vt_open_glove_port(char *dev_name, int baudrate);
00230     void vt_close_glove_port(CbGlove);
00231     CbGlove vt_create_CbGlove(Boolean connect, ...);
00232     void vt_destroy_CbGlove(CbGlove glove);
00233     void vt_glove_set(CbGlove glove);
00234     CbGlove vt_glove_get(void);
00235     float vt_get_glove_gain(int finger, int joint);
00236     void vt_set_glove_gain(int finger, int joint, float gain);
00237     int vt_get_glove_offset(int finger, int joint);
00238     void vt_set_glove_offset(int finger, int joint, int offset);
00239     void vt_unprocess_glove_angles(void);
00240     unsigned long vt_get_glove_param_flags(void);
00241     void vt_glove_private_fieldoffset(int finger, int joint,
00242                      int *offset, int *gain);
00243     int vt_read_glove_data(void);
00244     int vt_read_processed_glove_data(void);
00245     int vt_send_glove_command(int id, ...);
00246     int vt_send_glove_query(int id, ...);
00247     void vt_process_glove_data(void);
00248 
00249     int vt_serial_read_bytes(int, unsigned char *, int);
00250     int vt_serial_read_byte(int portfd);
00251     int vt_serial_clear_to_terminator(int portfd, int terminator);
00252     int vt_serial_read_longs(int portfd, void *buffer, int num_longs);
00253     int vt_serial_write_byte(int portfd, char ch);
00254     int vt_serial_write_bytes(int portfd, unsigned char buffer[], int numbytes);
00255     int vt_serial_write_shorts(int portfd, void *buffer, int num_shorts);
00256     int vt_serial_read_shorts(int portfd, void *buffer, int num_shorts);
00257     int vt_serial_read_string(int portfd, char *buffer, int length);
00258     int vt_serial_read_ushort(int portfd);
00259     char * vt_serial_read_line(int portfd, char *buffer, int length);
00260     int query_reply_head_check(int portfd, char *query_string);
00261     int reply_tail_check(CbGlove glove);
00262     int command_reply_check(CbGlove glove, char command_char);
00263     int read_and_decode_timestamp(int portfd, unsigned long *timestamp);
00264     int get_max_num_sensors(CbGlove glove, va_list args);
00265     CbGlove allocate_CbGlove(void);
00266     int glove_boot(CbGlove glove);
00267     int set_baud_rate(CbGlove glove, va_list args);
00268     int enable_timestamp(CbGlove glove, va_list args);
00269     int enable_filter(CbGlove glove, va_list args);
00270     int request_single_sample(CbGlove glove, va_list args);
00271     int request_sample_stream(CbGlove glove, va_list args);
00272     int enable_switch_controls_light(CbGlove glove, va_list args);
00273     int set_sensor_mask(CbGlove glove, va_list args);
00274     int set_num_values_returned(CbGlove glove, va_list args);
00275     int set_param_flags(CbGlove glove, va_list args);
00276     int reboot_CGIU(CbGlove glove, va_list args);
00277 
00278     int glove_ok_query(CbGlove glove, va_list args);
00279     int get_glove_info(CbGlove glove, va_list args);
00280     int get_avail_sensor_mask(CbGlove glove, va_list args);
00281     int get_sensor_mask(CbGlove glove, va_list args);
00282     int get_num_values_returned(CbGlove glove, va_list args);
00283     int read_param_flags(CbGlove glove, va_list args);
00284     int right_hand_glove_query(CbGlove glove, va_list args);
00285     void abduct_18 (void);
00286 
00287     // vt_serial_io.h
00288     int vt_serial_open(char *devname, int baudrate);
00289     int vt_serial_write_string(int portfd, char *str);
00290     char vt_serial_find_byte(int portfd, char delimiter);
00291     int vt_serial_flush_in(int portfd);
00292     void vt_serial_read_all(int portfd);
00293     int vt_serial_read_int(int portfd);
00294     int vt_serial_close(int portfd);
00295     void vt_serial_close_ports(void);
00296     int vt_serial_match_string(int portfd, char *str);
00297 
00298     // vt_read_hand_model
00299     void vt_read_hand_model(char infilename[], VirtualHand hand, char dir[]);
00300     void vt_read_lowres_hand_model(char infilename[], VirtualHand hand);
00301 
00302     // vt_read_object
00303     loopsegptr vt_loopseg_twin(loopsegptr theloopseg);
00304     void vt_read_object(FILE *infile, objptr thisobj);
00305     void vt_calculate_vertex_normals(objptr thisobj);
00306     void vt_calculate_dihedral_angles(objptr thisobj);
00307     void vt_calculate_face_normals(objptr thisobj, Boolean clockwise);
00308 
00309 };
00310 
00311 #endif  /* _CYBERGLOVE_H_ */
