#include "algorithm.h"

void maxim_heart_rate_and_oxygen_saturation(unsigned int *pun_ir_buffer,  int n_ir_buffer_length, unsigned int *pun_red_buffer, int *pn_spo2, int *pch_spo2_valid, 
		int *pn_heart_rate, int *pch_hr_valid)
{
#if 1
	unsigned int un_ir_mean ,un_only_once ;
	int k ,n_i_ratio_count;
	int i,s ,m, n_exact_ir_valley_locs_count ,n_middle_idx;
	int n_th1, n_npks,n_c_min;      
	int an_ir_valley_locs[15] ;
	int an_exact_ir_valley_locs[15] ;
	int an_dx_peak_locs[15] ;
	int n_peak_interval_sum;

	int n_y_ac, n_x_ac;
	int n_spo2_calc; 
	int n_y_dc_max, n_x_dc_max; 
	int n_y_dc_max_idx, n_x_dc_max_idx; 
	int an_ratio[5],n_ratio_average; 
	int n_nume,  n_denom ;
	// remove DC of ir signal    
	un_ir_mean =0; 
	for (k=0 ; k < n_ir_buffer_length ; k++ ) 
		un_ir_mean += pun_ir_buffer[k] ;

	un_ir_mean =un_ir_mean/n_ir_buffer_length ;

	for (k=0 ; k<n_ir_buffer_length ; k++ ) {  
		an_x[k] =  pun_ir_buffer[k] - un_ir_mean ; 
	}

	// 4 pt Moving Average
	for(k=0; k< BUFFER_SIZE-MA4_SIZE; k++){
		n_denom= ( an_x[k]+an_x[k+1]+ an_x[k+2]+ an_x[k+3]);
		an_x[k]=  n_denom/(int)4; 
	}

	// get difference of smoothed IR signal

	for( k=0; k<BUFFER_SIZE-MA4_SIZE-1;  k++) {
		an_dx[k]= (an_x[k+1]- an_x[k]);
	}

	// 2-pt Moving Average to an_dx
	for(k=0; k< BUFFER_SIZE-MA4_SIZE-2; k++){
		an_dx[k] =  ( an_dx[k]+an_dx[k+1])/2 ;
	}

	// hamming window
	// flip wave form so that we can detect valley with peak detector
	for ( i=0 ; i<BUFFER_SIZE-HAMMING_SIZE-MA4_SIZE-2 ;i++){
		s= 0;
		for( k=i; k<i+ HAMMING_SIZE ;k++){
			s -= an_dx[k] *auw_hamm[k-i] ; 
		}
		an_dx[i]= s/ (int)1146; // divide by sum of auw_hamm 
	}


	n_th1=0; // threshold calculation
	for ( k=0 ; k<BUFFER_SIZE-HAMMING_SIZE ;k++){
		n_th1 += ((an_dx[k]>0)? an_dx[k] : ((int)0-an_dx[k])) ;
	}
	n_th1= n_th1/ ( BUFFER_SIZE-HAMMING_SIZE);
	// peak location is acutally index for sharpest location of raw signal since we flipped the signal         
	maxim_find_peaks( an_dx_peak_locs, &n_npks, an_dx, BUFFER_SIZE-HAMMING_SIZE, n_th1, 8, 5 );//peak_height, peak_distance, max_num_peaks 

	n_peak_interval_sum =0;
	if (n_npks>=2){
		for (k=1; k<n_npks; k++)
			n_peak_interval_sum += (an_dx_peak_locs[k]-an_dx_peak_locs[k -1]);
		n_peak_interval_sum=n_peak_interval_sum/(n_npks-1);
		*pn_heart_rate=(int)(6000/n_peak_interval_sum);// beats per minutes
		*pch_hr_valid  = 1;
	}
	else  {
		*pn_heart_rate = -999;
		*pch_hr_valid  = 0;
	}

	for ( k=0 ; k<n_npks ;k++)
		an_ir_valley_locs[k]=an_dx_peak_locs[k]+HAMMING_SIZE/2; 


	// raw value : RED(=y) and IR(=X)
	// we need to assess DC and AC value of ir and red PPG. 
	for (k=0 ; k<n_ir_buffer_length ; k++ )  {
		an_x[k] =  pun_ir_buffer[k] ; 
		an_y[k] =  pun_red_buffer[k] ; 
	}

	// find precise min near an_ir_valley_locs
	n_exact_ir_valley_locs_count =0; 
	for(k=0 ; k<n_npks ;k++){
		un_only_once =1;
		m=an_ir_valley_locs[k];
		n_c_min= 16777216;//2^24;
		if (m+5 <  BUFFER_SIZE-HAMMING_SIZE  && m-5 >0){
			for(i= m-5;i<m+5; i++)
				if (an_x[i]<n_c_min){
					if (un_only_once >0){
						un_only_once =0;
					} 
					n_c_min= an_x[i] ;
					an_exact_ir_valley_locs[k]=i;
				}
			if (un_only_once ==0)
				n_exact_ir_valley_locs_count ++ ;
		}
	}
	if (n_exact_ir_valley_locs_count <2 ){
		*pn_spo2 =  -999 ; // do not use SPO2 since signal ratio is out of range
		*pch_spo2_valid  = 0; 
		return;
	}
	// 4 pt MA
	for(k=0; k< BUFFER_SIZE-MA4_SIZE; k++){
		an_x[k]=( an_x[k]+an_x[k+1]+ an_x[k+2]+ an_x[k+3])/(int)4;
		an_y[k]=( an_y[k]+an_y[k+1]+ an_y[k+2]+ an_y[k+3])/(int)4;
	}

	//using an_exact_ir_valley_locs , find ir-red DC andir-red AC for SPO2 calibration ratio
	//finding AC/DC maximum of raw ir * red between two valley locations
	n_ratio_average =0; 
	n_i_ratio_count =0; 

	for(k=0; k< 5; k++) an_ratio[k]=0;
	for (k=0; k< n_exact_ir_valley_locs_count; k++){
		if (an_exact_ir_valley_locs[k] > BUFFER_SIZE ){             
			*pn_spo2 =  -999 ; // do not use SPO2 since valley loc is out of range
			*pch_spo2_valid  = 0; 
			return;
		}
	}
	// find max between two valley locations 
	// and use ratio betwen AC compoent of Ir & Red and DC compoent of Ir & Red for SPO2 

	for (k=0; k< n_exact_ir_valley_locs_count-1; k++){
		n_y_dc_max= -16777216 ; 
		n_x_dc_max= - 16777216; 
		if (an_exact_ir_valley_locs[k+1]-an_exact_ir_valley_locs[k] >10){
			for (i=an_exact_ir_valley_locs[k]; i< an_exact_ir_valley_locs[k+1]; i++){
				if (an_x[i]> n_x_dc_max) {n_x_dc_max =an_x[i];n_x_dc_max_idx =i; }
				if (an_y[i]> n_y_dc_max) {n_y_dc_max =an_y[i];n_y_dc_max_idx=i;}
			}
			n_y_ac= (an_y[an_exact_ir_valley_locs[k+1]] - an_y[an_exact_ir_valley_locs[k] ] )*(n_y_dc_max_idx -an_exact_ir_valley_locs[k]); //red
			n_y_ac=  an_y[an_exact_ir_valley_locs[k]] + n_y_ac/ (an_exact_ir_valley_locs[k+1] - an_exact_ir_valley_locs[k])  ; 


			n_y_ac=  an_y[n_y_dc_max_idx] - n_y_ac;    // subracting linear DC compoenents from raw 
			n_x_ac= (an_x[an_exact_ir_valley_locs[k+1]] - an_x[an_exact_ir_valley_locs[k] ] )*(n_x_dc_max_idx -an_exact_ir_valley_locs[k]); // ir
			n_x_ac=  an_x[an_exact_ir_valley_locs[k]] + n_x_ac/ (an_exact_ir_valley_locs[k+1] - an_exact_ir_valley_locs[k]); 
			n_x_ac=  an_x[n_y_dc_max_idx] - n_x_ac;      // subracting linear DC compoenents from raw 
			n_nume=( n_y_ac *n_x_dc_max)>>7 ; //prepare X100 to preserve floating value
			n_denom= ( n_x_ac *n_y_dc_max)>>7;
			if (n_denom>0  && n_i_ratio_count <5 &&  n_nume != 0)
			{   
				an_ratio[n_i_ratio_count]= (n_nume*100)/n_denom ; //formular is ( n_y_ac *n_x_dc_max) / ( n_x_ac *n_y_dc_max) ;
				n_i_ratio_count++;
			}
		}
	}

	maxim_sort_ascend(an_ratio, n_i_ratio_count);
	n_middle_idx= n_i_ratio_count/2;

	if (n_middle_idx >1)
		n_ratio_average =( an_ratio[n_middle_idx-1] +an_ratio[n_middle_idx])/2; // use median
	else
		n_ratio_average = an_ratio[n_middle_idx ];

	if( n_ratio_average>2 && n_ratio_average <184){
		n_spo2_calc= uch_spo2_table[n_ratio_average] ;
		*pn_spo2 = n_spo2_calc ;
		*pch_spo2_valid  = 1;//  float_SPO2 =  -45.060*n_ratio_average* n_ratio_average/10000 + 30.354 *n_ratio_average/100 + 94.845 ;  // for comparison with table
	}
	else{
		*pn_spo2 =  -999 ; // do not use SPO2 since signal ratio is out of range
		*pch_spo2_valid  = 0; 
	}
#else

  unsigned int un_ir_mean,un_only_once ;
  int k, n_i_ratio_count;
  int i, s, m, n_exact_ir_valley_locs_count, n_middle_idx;
  int n_th1, n_npks, n_c_min;   
  int an_ir_valley_locs[15] ;
  int n_peak_interval_sum;
  
  int n_y_ac, n_x_ac;
  int n_spo2_calc; 
  int n_y_dc_max, n_x_dc_max; 
  int n_y_dc_max_idx, n_x_dc_max_idx; 
  int an_ratio[5], n_ratio_average; 
  int n_nume, n_denom ;

  // calculates DC mean and subtract DC from ir
  un_ir_mean =0; 
  for (k=0 ; k<n_ir_buffer_length ; k++ ) un_ir_mean += pun_ir_buffer[k] ;
  un_ir_mean =un_ir_mean/n_ir_buffer_length ;
    
  // remove DC and invert signal so that we can use peak detector as valley detector
  for (k=0 ; k<n_ir_buffer_length ; k++ )  
    an_x[k] = -1*(pun_ir_buffer[k] - un_ir_mean) ; 
    
  // 4 pt Moving Average
  for(k=0; k< n_ir_buffer_length-MA4_SIZE; k++){
    an_x[k]=( an_x[k]+an_x[k+1]+ an_x[k+2]+ an_x[k+3])/(int)4;        
  }
  // calculate threshold  
  n_th1=0; 
  for ( k=0 ; k<n_ir_buffer_length ;k++){
    n_th1 +=  an_x[k];
  }
  n_th1=  n_th1/ n_ir_buffer_length;
  if( n_th1<30) n_th1=30; // min allowed
  if( n_th1>60) n_th1=60; // max allowed

  for ( k=0 ; k<15;k++) 
	  an_ir_valley_locs[k]=0;
  // since we flipped signal, we use peak detector as valley detector
  maxim_find_peaks(an_ir_valley_locs, &n_npks, an_x, n_ir_buffer_length, n_th1, 4, 15 );//peak_height, peak_distance, max_num_peaks 

  n_peak_interval_sum =0;
  if (n_npks>=2){
    for (k=1; k<n_npks; k++) n_peak_interval_sum += (an_ir_valley_locs[k] -an_ir_valley_locs[k -1] ) ;
    n_peak_interval_sum =n_peak_interval_sum/(n_npks-1);
    *pn_heart_rate =(int)((FS*60)/ n_peak_interval_sum );
    *pch_hr_valid  = 1;
  }
  else  { 
    *pn_heart_rate = -999; // unable to calculate because # of peaks are too small
    *pch_hr_valid  = 0;
  }

  //  load raw value again for SPO2 calculation : RED(=y) and IR(=X)
  for (k=0 ; k<n_ir_buffer_length ; k++ )  {
      an_x[k] =  pun_ir_buffer[k] ; 
      an_y[k] =  pun_red_buffer[k] ; 
  }

  // find precise min near an_ir_valley_locs
  n_exact_ir_valley_locs_count =n_npks; 
  
  //using exact_ir_valley_locs , find ir-red DC andir-red AC for SPO2 calibration an_ratio
  //finding AC/DC maximum of raw

  n_ratio_average =0; 
  n_i_ratio_count = 0; 
  for(k=0; k< 5; k++) an_ratio[k]=0;
  for (k=0; k< n_exact_ir_valley_locs_count; k++){
    if (an_ir_valley_locs[k] > n_ir_buffer_length ){
      *pn_spo2 =  -999 ; // do not use SPO2 since valley loc is out of range
      *pch_spo2_valid  = 0; 
      return;
    }
  }
  // find max between two valley locations 
  // and use an_ratio betwen AC compoent of Ir & Red and DC compoent of Ir & Red for SPO2 
  for (k=0; k< n_exact_ir_valley_locs_count-1; k++){
    n_y_dc_max= -16777216 ; 
    n_x_dc_max= -16777216; 
    if (an_ir_valley_locs[k+1]-an_ir_valley_locs[k] >3){
        for (i=an_ir_valley_locs[k]; i< an_ir_valley_locs[k+1]; i++){
          if (an_x[i]> n_x_dc_max) {n_x_dc_max =an_x[i]; n_x_dc_max_idx=i;}
          if (an_y[i]> n_y_dc_max) {n_y_dc_max =an_y[i]; n_y_dc_max_idx=i;}
      }
      n_y_ac= (an_y[an_ir_valley_locs[k+1]] - an_y[an_ir_valley_locs[k] ] )*(n_y_dc_max_idx -an_ir_valley_locs[k]); //red
      n_y_ac=  an_y[an_ir_valley_locs[k]] + n_y_ac/ (an_ir_valley_locs[k+1] - an_ir_valley_locs[k])  ; 
      n_y_ac=  an_y[n_y_dc_max_idx] - n_y_ac;    // subracting linear DC compoenents from raw 
      n_x_ac= (an_x[an_ir_valley_locs[k+1]] - an_x[an_ir_valley_locs[k] ] )*(n_x_dc_max_idx -an_ir_valley_locs[k]); // ir
      n_x_ac=  an_x[an_ir_valley_locs[k]] + n_x_ac/ (an_ir_valley_locs[k+1] - an_ir_valley_locs[k]); 
      n_x_ac=  an_x[n_y_dc_max_idx] - n_x_ac;      // subracting linear DC compoenents from raw 
      n_nume=( n_y_ac *n_x_dc_max)>>7 ; //prepare X100 to preserve floating value
      n_denom= ( n_x_ac *n_y_dc_max)>>7;
      if (n_denom>0  && n_i_ratio_count <5 &&  n_nume != 0)
      {   
        an_ratio[n_i_ratio_count]= (n_nume*100)/n_denom ; //formular is ( n_y_ac *n_x_dc_max) / ( n_x_ac *n_y_dc_max) ;
        n_i_ratio_count++;
      }
    }
  }
  // choose median value since PPG signal may varies from beat to beat
  maxim_sort_ascend(an_ratio, n_i_ratio_count);
  n_middle_idx= n_i_ratio_count/2;

  if (n_middle_idx >1)
    n_ratio_average =( an_ratio[n_middle_idx-1] +an_ratio[n_middle_idx])/2; // use median
  else
    n_ratio_average = an_ratio[n_middle_idx ];

  if( n_ratio_average>2 && n_ratio_average <184){
    n_spo2_calc= uch_spo2_table[n_ratio_average] ;
    *pn_spo2 = n_spo2_calc ;
    *pch_spo2_valid  = 1;//  float_SPO2 =  -45.060*n_ratio_average* n_ratio_average/10000 + 30.354 *n_ratio_average/100 + 94.845 ;  // for comparison with table
  }
  else{
    *pn_spo2 =  -999 ; // do not use SPO2 since signal an_ratio is out of range
    *pch_spo2_valid  = 0; 
  }
#endif
}


void maxim_find_peaks(int *pn_locs, int *pn_npks, int *pn_x, int n_size, int n_min_height, int n_min_distance, int n_max_num)
{
	maxim_peaks_above_min_height( pn_locs, pn_npks, pn_x, n_size, n_min_height );
	maxim_remove_close_peaks( pn_locs, pn_npks, pn_x, n_min_distance );
	*pn_npks = min( *pn_npks, n_max_num );
}

void maxim_peaks_above_min_height(int *pn_locs, int *pn_npks, int  *pn_x, int n_size, int n_min_height)
{
	int i = 1, n_width;
	*pn_npks = 0;

	while (i < n_size-1){
		if (pn_x[i] > n_min_height && pn_x[i] > pn_x[i-1]){            // find left edge of potential peaks
			n_width = 1;
			while (i+n_width < n_size && pn_x[i] == pn_x[i+n_width])    // find flat peaks
				n_width++;
			if (pn_x[i] > pn_x[i+n_width] && (*pn_npks) < 15 ){                            // find right edge of peaks
				pn_locs[(*pn_npks)++] = i;        
				// for flat peaks, peak location is left edge
				i += n_width+1;
			}
			else
				i += n_width;
		}
		else
			i++;
	}
}


void maxim_remove_close_peaks(int *pn_locs, int *pn_npks, int *pn_x,int n_min_distance)
{

	int i, j, n_old_npks, n_dist;

	/* Order peaks from large to small */
	maxim_sort_indices_descend( pn_x, pn_locs, *pn_npks );

	for ( i = -1; i < *pn_npks; i++ ){
		n_old_npks = *pn_npks;
		*pn_npks = i+1;
		for ( j = i+1; j < n_old_npks; j++ ){
			n_dist =  pn_locs[j] - ( i == -1 ? -1 : pn_locs[i] ); // lag-zero peak of autocorr is at index -1
			if ( n_dist > n_min_distance || n_dist < -n_min_distance )
				pn_locs[(*pn_npks)++] = pn_locs[j];
		}
	}

	// Resort indices longo ascending order
	maxim_sort_ascend( pn_locs, *pn_npks );
}

void maxim_sort_ascend(int *pn_x,int n_size) 
{
	int i, j, n_temp;
	for (i = 1; i < n_size; i++) {
		n_temp = pn_x[i];
		for (j = i; j > 0 && n_temp < pn_x[j-1]; j--)
			pn_x[j] = pn_x[j-1];
		pn_x[j] = n_temp;
	}
}

void maxim_sort_indices_descend(int *pn_x, int *pn_indx, int n_size)
{
	int i, j, n_temp;
	for (i = 1; i < n_size; i++) {
		n_temp = pn_indx[i];
		for (j = i; j > 0 && pn_x[n_temp] > pn_x[pn_indx[j-1]]; j--)
			pn_indx[j] = pn_indx[j-1];
		pn_indx[j] = n_temp;
	}
}
