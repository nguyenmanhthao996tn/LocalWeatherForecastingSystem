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
	FILE *fp_w;
	double data_input[14],min_temp=1000,max_temp=0,rain,atm_dec=0,atm_dec_past=0,atm_past,atm_curr=0,min_hum=1000,max_hum=0,wind,tmp_atm;
	int i,count=0,tmp,h_tmp=-1,month,wind_dir, count_atm=0, count_temp=0, count_hum=0;;
	double arr_atm[24], arr_temp[12], arr_hum[12];
	fp_r = fopen(fname,"r");
	fp_w = fopen("dedata_v7_4.txt", "w+");
	if(!fp_r) return 0;
	
	for(i =0;i<24;i++) arr_atm[i] = 0;
	for(i =0;i<12;i++) arr_temp[i] = -1;
	for(i =0;i<12;i++) arr_hum[i] = -1;
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
			count_atm++;
			
			// calculator temp
			if(count_temp < 12) 
			{
				if (data_input[8] == 0) arr_temp[count_temp] = arr_temp[count_temp-1];
				else arr_temp[count_temp] = data_input[8];
				min_temp = 1000;
				max_temp = 0;
				for(i=0;i<12;i++)
				{
					if(arr_temp[i] >0)
					{
						if(arr_temp[i] < min_temp) min_temp = arr_temp[i];
						if(arr_temp[i] > max_temp) max_temp = arr_temp[i];
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
				min_temp = 1000;
				max_temp = 0;
				for(i=0;i<12;i++)
				{
					if(arr_temp[i] >0)
					{
						if(arr_temp[i] < min_temp) min_temp = arr_temp[i];
						if(arr_temp[i] > max_temp) max_temp = arr_temp[i];
					}
				}
			}
			count_temp++;
				
			// calculator hum
			if(count_hum < 12) 
			{
				if (data_input[10] <= 0) arr_hum[count_hum] = arr_hum[count_hum-1];
				else arr_hum[count_hum] = data_input[10];
				min_hum = 1000;
				max_hum = 0;
				for(i=0;i<12;i++)
				{
					if(arr_hum[i] >0)
					{
						if(arr_hum[i] < min_hum) min_hum = arr_hum[i];
						if(arr_hum[i] > max_hum) max_hum = arr_hum[i];
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
				min_hum = 1000;
				max_hum = 0;
				for(i=0;i<12;i++)
				{
					if(arr_hum[i] >0)
					{
						if(arr_hum[i] < min_hum) min_hum = arr_hum[i];
						if(arr_hum[i] > max_hum) max_hum = arr_hum[i];
					}
				}
			}
			count_hum++;
			
			// wind_dir
			if ((data_input[5] < 180) && (data_input[5] >= 0 )) wind_dir = 1; else wind_dir = 0;	
			
			// wind
			wind = data_input[6];
			
			// hour
			h_tmp = data_input[3];
			
			fprintf(fp_w," %.3f %d %d %.3f %.3f %.3f %.3f %.3f %.3f\n",wind/10,wind_dir,h_tmp,min_temp/10,max_temp/10,atm_dec/10,min_hum/10,max_hum/10,rain/100.0);
			
			
			
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
			count = 0;
		}
	  }
      else
	  {
	  	break;
	  }
    }
	
	if(fp_r) fclose(fp_r);
	fclose(fp_w);
	  return 1;
}
