#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Proccess_data.h"

bool read_number(FILE* fp, double* number)
{
  char szWord[256];
  int i = 0;
  int b;

  *number = 0.0;

  szWord[0] = '\0';
  while ( ((b=fgetc(fp))!=EOF) && (i<255) )
    {
      if( //(b=='-') ||
	  (b=='.') ||
	  (b=='0') ||
	  (b=='1') ||
	  (b=='2') ||
	  (b=='3') ||
	  (b=='4') ||
	  (b=='5') ||
	  (b=='6') ||
	  (b=='7') ||
	  (b=='8') ||
	  (b=='9') )
	{
	  szWord[i++] = (char)b;
	}
      else
	if(i>0) break;
    }
  szWord[i] = '\0';

  if(i==0) return false;

  *number = atof(szWord);

  return true;
}

int Read_Write_file_data(const char* fname) {
	FILE*   fp_r = NULL;
	FILE*   fp_r2 = NULL;	
	FILE *fp_w;
	double data_input[14],avg_temp=0,atm_dec=0,atm_dec_past=0,atm_past,atm_curr=0,avg_hum=0,wind,tmp_atm,h_tmp=-1,wind_dir;
	int i,count=0,count_rain=0,count_read=0,tmp,month, count_atm=0, count_temp=0, count_hum=0,rain;
	double arr_atm[24], arr_temp[12], arr_hum[12];
	int arr_a[3000];
	int rain_num=0;
	double max_wind=0,max_temp=0,max_hum=0,max_atm=0;
	
	// create array rain
	fp_r2 = fopen(fname,"r");
//	fp_w = fopen("KQ_rain_9.txt", "w+");
	if(!fp_r2) return 0;
	
	while( !feof(fp_r2) ) 
	{
      double dNumber;
      if( read_number(fp_r2,&dNumber) )
	  {
		data_input[count]=dNumber;
		count++;
		if(count >= 13) 
		{
			int data_tmp = data_input[9];
			if(data_tmp > 2 ) 
				arr_a[count_rain] = 1;
			else 
				arr_a[count_rain] = 0;
//			int month = data_input[1];
//			int day = data_input[2];
//			int hour = data_input[3];
//			int min = data_input[4];
//			fprintf(fp_w," %d-%d %d:%d %d\n", month,day,hour,min,arr_a[count_rain]);
			count_rain++;
			count = 0;	
		}
	  } else {
	  	 break;
		}
	} 
	if(fp_r2) fclose(fp_r2);
//	fclose(fp_w);

	// main process
	fp_r = fopen(fname,"r");
	fp_w = fopen("train_v1_10.txt", "w+");
	if(!fp_r) return 0;
	
	for(i =0;i<24;i++) arr_atm[i] = 0;
	for(i =0;i<12;i++) arr_temp[i] = 0;
	for(i =0;i<12;i++) arr_hum[i] = 0;
	count=0;
	
	while( !feof(fp_r) ) 
	{
      double dNumber;
      if( read_number(fp_r,&dNumber) )
	  {
		data_input[count]=dNumber;
		count++;
		if(count >= 13) 
		{ 
			tmp = data_input[12];
			data_input[12] = data_input[9];
			data_input[9] = tmp;
			tmp = data_input[11];
			data_input[11] = data_input[10];
			data_input[10] = tmp;
			data_input[8] = (data_input[8] - 32) / 1.8;	
			data_input[9] -= 9900;
			
			// data v3
			// input: hour, wind, wind_dir, min_temp, max_temp, min_hum, max_hum, atm
			// output: rain
			
			// calculator atm
			if(count_atm < 24) 
			{
				if (data_input[9] <= 0) arr_atm[count_atm] = arr_atm[count_atm-1];
				else arr_atm[count_atm] = data_input[9];
				atm_dec=0;
				for(i=1;i<24;i++)
				{
					if(arr_atm[i] != 0) atm_dec += arr_atm[i-1] - arr_atm[i];
				}
			} else {
				for(i=0;i<23;i++)
				{
					arr_atm[i] = arr_atm[i+1];
				}
				count_atm= count_atm -1;
				if (data_input[9] <= 0) arr_atm[count_atm] = arr_atm[count_atm-1];
				else arr_atm[count_atm] = data_input[9];
				atm_dec=0;
				for(i=1;i<24;i++)
				{
					atm_dec += arr_atm[i-1] - arr_atm[i];
				}
			}
			if ( abs(atm_dec)> max_atm) max_atm = abs(atm_dec);
			count_atm++;
			
			// calculator temp
			if(count_temp < 12) 
			{
				if (data_input[8] <= 0) arr_temp[count_temp] = arr_temp[count_temp-1];
				else arr_temp[count_temp] = data_input[8];
				avg_temp = 0;
				for(i=1;i<12;i++)
				{
					if(arr_temp[i] != 0){
						avg_temp += arr_temp[i-1] - arr_temp[i];
					}
				}
			} else {
				for(i=0;i<11;i++)
				{
					arr_temp[i] = arr_temp[i+1];
				}
				count_temp = count_temp -1;
				if (data_input[8] <= 0) arr_temp[count_temp] = arr_temp[count_temp-1];
				else arr_temp[count_temp] = data_input[8];
				avg_temp = 0;
				for(i=1;i<12;i++)
				{
					avg_temp += arr_temp[i-1] - arr_temp[i];
				}
			}
			if(abs(avg_temp) > max_temp) max_temp = abs(avg_temp);
			count_temp++;
				
			// calculator hum
			if(count_hum < 12) 
			{
				if (data_input[10] <= 0) arr_hum[count_hum] = arr_hum[count_hum-1];
				else arr_hum[count_hum] = data_input[10];
				avg_hum = 0;
				for(i=1;i<12;i++)
				{
					if(arr_hum[i] != 0) {
						avg_hum += arr_hum[i-1] - arr_hum[i];
					}
				}
			} else {
				for(i=0;i<11;i++)
				{
					arr_hum[i] = arr_hum[i+1];
				}
				count_hum = count_hum -1;
				if (data_input[10] <= 0) arr_hum[count_hum] = arr_hum[count_hum-1];
				else arr_hum[count_hum] = data_input[10];
				avg_hum = 0;
				for(i=1;i<12;i++)
				{
					avg_hum += arr_hum[i-1] - arr_hum[i];
				}
			}
			if(abs(avg_hum) > max_hum) max_hum = abs(avg_hum);
			count_hum++;
			
			// wind_dir
//			if ((data_input[5] < 180) && (data_input[5] >= 0 )) wind_dir = 1; else wind_dir = 0;
			if (0 <= data_input[5] && data_input[5] < 30) wind_dir = 0;
			if (30 <= data_input[5] && data_input[5] < 60) wind_dir = 1;
			if (60 <= data_input[5] && data_input[5] < 90) wind_dir = 2;
			if (90 <= data_input[5] && data_input[5] < 120) wind_dir = 3;
			if (120 <= data_input[5] && data_input[5] < 150) wind_dir = 4;
			if (150 <= data_input[5] && data_input[5] < 180) wind_dir = 5;
			if (180 <= data_input[5] && data_input[5] < 210) wind_dir = 6;
			if (210 <= data_input[5] && data_input[5] < 240) wind_dir = 7;
			if (240 <= data_input[5] && data_input[5] < 270) wind_dir = 8;
			if (270 <= data_input[5] && data_input[5] < 300) wind_dir = 9;
			if (300 <= data_input[5] && data_input[5] < 330) wind_dir = 10;
			if (330 <= data_input[5] && data_input[5] < 360) wind_dir = 11;
			
			// wind
			wind = data_input[7];
			if(abs(wind) > max_wind) max_wind = abs(wind);
			
			// hour
			h_tmp = data_input[3];
			
			// rain
			if ((count_read+24) < count_rain) {
				int j=count_read+24;
				while ( (j< (count_read+36)) ) {
					if(j > count_rain) break;
					if(arr_a[j] > 0){
						rain_num++;
					}
					j++;
				}
				if ( rain_num > 3) 
					rain = 1;
				else 
					rain = 0;
				rain_num =0;
			}
			
			fprintf(fp_w," %.3f %.3f %.3f %.3f %.3f %.3f %d\n",h_tmp/24,wind/118,wind_dir/12,avg_temp/22,atm_dec/40,avg_hum/45,rain);
			
			
			
			// data v2
//			if(data_input[3] == h_tmp)
//			{
//				if (data_input[8] < min_temp) min_temp = data_input[8];
//				if (data_input[8] > max_temp) max_temp = data_input[8];
//				if (data_input[10] < min_hum) min_hum = data_input[10];
//				if (data_input[10] > max_hum) max_hum = data_input[10];
//				if (data_input[12] > rain) rain = data_input[12];
//				if (data_input[6] > wind) wind = data_input[6];
//				if ((data_input[5] < 180) && (data_input[5] >= 0 )) wind_dir = 1; else wind_dir = 0;
//				atm_dec += ( atm_curr - data_input[9]);
//				atm_curr = data_input[9];
//				month = data_input[1];
//			} 
//			else
//			{	
//				tmp_atm = atm_dec;
//				atm_dec += atm_dec_past;
//				atm_dec_past = tmp_atm;
//				if(h_tmp != -1)
//					fprintf(fp_w," %.3f %d %d %.3f %.3f %.3f %.3f %.3f %.3f\n",wind/10,wind_dir,h_tmp,min_temp/10,max_temp/10,atm_dec/10,min_hum/10,max_hum/10,rain/100.0);
//				h_tmp = data_input[3];
//				atm_curr = data_input[9];
//				atm_dec = 0;
//				min_hum = data_input[10];
//				max_hum = data_input[10];
//				min_temp = data_input[8];
//				max_temp = data_input[8];
//				rain = data_input[12];
//				wind = data_input[6];
//				month = data_input[1];
//				if ((data_input[5] < 180) && (data_input[5] >= 0 )) wind_dir = 1; else wind_dir = 0;
//				
//			}
			
			// data v1
//			for(int x=0;x<13;x++) 
//			{
//				fprintf(fp_w," %.2f",data_input[x]);
//			}
//			fprintf(fp_w,"\n");
			count_read++;
			count = 0;
		}
	  }
      else
	  {
	  	break;
	  }
    }
	
//	fprintf(fp_w," %.3f %.3f %.3f %.3f ",max_wind,max_temp,max_atm,max_hum);
	if(fp_r) fclose(fp_r);
	fclose(fp_w);
	  return 1;
}
