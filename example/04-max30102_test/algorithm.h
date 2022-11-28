#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#define true 1
#define false 0
#define FS 100
#define BUFFER_SIZE  (FS* 5) 
//#define BUFFER_SIZE  (FS* 2) 
#define HR_FIFO_SIZE 7
#define MA4_SIZE  4 // DO NOT CHANGE
#define HAMMING_SIZE  5// DO NOT CHANGE
#define min(x,y) ((x) < (y) ? (x) : (y))

const unsigned short auw_hamm[31]={ 41,    276,    512,    276,     41 };

const unsigned char uch_spo2_table[184]={ 95, 95, 95, 96, 96, 96, 97, 97, 97, 97, 97, 98, 98, 98, 98, 98, 99, 99, 99, 99, 
                            99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
                            100, 100, 100, 100, 99, 99, 99, 99, 99, 99, 99, 99, 98, 98, 98, 98, 98, 98, 97, 97, 
                            97, 97, 96, 96, 96, 96, 95, 95, 95, 94, 94, 94, 93, 93, 93, 92, 92, 92, 91, 91, 
                            90, 90, 89, 89, 89, 88, 88, 87, 87, 86, 86, 85, 85, 84, 84, 83, 82, 82, 81, 81, 
                            80, 80, 79, 78, 78, 77, 76, 76, 75, 74, 74, 73, 72, 72, 71, 70, 69, 69, 68, 67, 
                            66, 66, 65, 64, 63, 62, 62, 61, 60, 59, 58, 57, 56, 56, 55, 54, 53, 52, 51, 50, 
                            49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 31, 30, 29, 
                            28, 27, 26, 25, 23, 22, 21, 20, 19, 17, 16, 15, 14, 12, 11, 10, 9, 7, 6, 5, 
                            3, 2, 1 } ;

static  int an_dx[BUFFER_SIZE-MA4_SIZE];
static  int an_x[ BUFFER_SIZE]; //ir
static  int an_y[ BUFFER_SIZE]; //red


void maxim_heart_rate_and_oxygen_saturation(unsigned int *pun_ir_buffer ,  int n_ir_buffer_length, unsigned int *pun_red_buffer ,   int *pn_spo2, int *pch_spo2_valid ,  int *pn_heart_rate , int  *pch_hr_valid);
void maxim_find_peaks( int *pn_locs, int *pn_npks,  int *pn_x, int n_size, int n_min_height, int n_min_distance, int n_max_num );
void maxim_peaks_above_min_height( int *pn_locs, int *pn_npks,  int *pn_x, int n_size, int n_min_height );
void maxim_remove_close_peaks( int *pn_locs, int *pn_npks,   int  *pn_x, int n_min_distance );
void maxim_sort_ascend( int *pn_x, int n_size );
void maxim_sort_indices_descend(  int  *pn_x, int *pn_indx, int n_size);

#endif /* ALGORITHM_H_ */
