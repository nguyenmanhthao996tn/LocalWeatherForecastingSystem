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
	double data_input[14],min_temp,max_temp,rain;
	int count=0,tmp,min_atm,max_atm,h_tmp=-1,min_hum,max_hum,month;
	fp_r = fopen(fname,"r");
	fp_w = fopen("dedata_v2_1.txt", "w+");
	if(!fp_r) return 0;
	
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
			// data v2
			if(data_input[3] == h_tmp)
			{
				if (data_input[8] < min_temp) min_temp = data_input[8];
				if (data_input[8] > max_temp) max_temp = data_input[8];
				if (data_input[9] < min_atm) min_atm = data_input[9];
				if (data_input[9] > max_atm) max_atm = data_input[9];
				if (data_input[10] < min_hum) min_hum = data_input[10];
				if (data_input[10] > max_hum) max_hum = data_input[10];
				rain = data_input[12];
				month = data_input[1];
			} 
			else
			{	
				if(h_tmp != -1)
				fprintf(fp_w," %d %d %.2f %.2f %d %d %d %d %.2f\n",month,h_tmp,min_temp,max_temp,min_atm,max_atm,min_hum,max_hum,rain/100.0);
				h_tmp = data_input[3];
				min_atm = min_hum = 32000;
				max_atm = max_hum = 0;
				min_temp = 32000.0;
				max_temp = 0.0;
			}
			
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
