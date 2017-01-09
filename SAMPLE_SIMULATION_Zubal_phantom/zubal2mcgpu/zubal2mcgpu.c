
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Structure with the info for each voxel number read from the phantom description file (ie, values that will be found in the raw data):
struct conversion_table       
{
  int   mc_material;       // Material number in the Monte Carlo simulation
  float density;           // Material density read from the phantom description file
};



/**********************************************************************
 *   Program to convert the Zubal body phantom to the MCGPU/penEasy   *
 *   voxel format.                                                    *
 *                                                                    *
 *   See: http://noodle.med.yale.edu/zubal/                           *
 *        http://code.google.com/p/mcgpu/                             *
 *                                                                    *
 *   The program expects 2 file names as a command line arguments:    *
 *     1) Name of the conversion table with data to assign the organs *
 *         to Monte Carlo material numbers.                           *
 *     2) Phantom binary file name.                                   *  
 *                                                                    * 
 *                            Andreu Badal, 2011-08-19                *
 **********************************************************************/
int main(int argc, char * argv[])
{
  
  printf("\n    ** Converting the Zubal phantom to the MCGPU/penEasy voxel format **\n");
  printf(  "       The phantom file (voxel_man.dat) can be dowloaded from http://noodle.med.yale.edu/zubal\n");
  printf(  "       The conversion table can be modified to change the number of materials that will be used in the simulation\n\n");
  
  if (argc != 3)
  {
    printf("  Usage:  ./zubal2mcgpu.x conversion_table.in phantom.raw\n");
    printf("   (Output will be written to file: phantom.raw.vox)\n\n");
    exit(0);
  }

  int organ_id, mc_material, num_voxels_x, num_voxels_y, num_voxels_z;  
  float density, size_voxels_x, size_voxels_y, size_voxels_z;
  char new_line[250], organ_name[30], *dummy;

  struct conversion_table table[150];  // Variable storing the conversion table for 150 possible organs.
  
  // Init table with silly values:
  for(organ_id=0; organ_id<150; organ_id++)
  {
    table[organ_id].mc_material = -1; 
    table[organ_id].density = -99.99;
  }  
  
  
  FILE* file_conversion = fopen(argv[1], "r");

  
  // -- Read voxel number and size:
  for(;;)
  {
    if (fgets(new_line, 250, file_conversion) == NULL)
    {
      printf("\n\n   !!ERROR reading file \'%s\'!!\n\n",argv[1]);
      exit(1);
    }
          //  printf("%s",new_line);      // !!DeBuG!!
    if (new_line[0]=='\n' || new_line[1]=='\n')
    {
      continue;  // Skip empty lines (or lines with just a blank character)
    }
    else if (strstr(new_line,"SECTION VOXELS") != NULL)
    {
      // -- Store the number of voxels and voxel size:
      dummy=fgets(new_line, 250, file_conversion);  sscanf(new_line, "%d %d %d",    &num_voxels_x, &num_voxels_y, &num_voxels_z);
      dummy=fgets(new_line, 250, file_conversion);  sscanf(new_line, "%f %f %f", &size_voxels_x, &size_voxels_y, &size_voxels_z);

      printf("\n    Number of voxels: nx=%d, ny=%d, nz=%d\n" , num_voxels_x,num_voxels_y,num_voxels_z);
      printf(  "    Voxel size [cm]:  Dx=%.3f, Dy=%.3f, Dz=%.3f\n" , size_voxels_x,size_voxels_y,size_voxels_z);
    }
    else if (strstr(new_line,"ORGAN_NAME") != NULL)
    {
      break;  // We arrived at the begining of the conversion table 
    }    
  }
  
  
  // -- Read conversion table:
  for(;;)
  {
    if (fgets(new_line, 250, file_conversion) == NULL)
    {
      printf("\n\n   !!ERROR reading file \'%s\'!!\n\n",argv[1]);
      exit(1);
    }
    if (new_line[0]=='\n' || new_line[1]=='\n')
      continue;  // Skip empty lines (or lines with just a blank character)
    else if (strstr(new_line,"Monte Carlo material") != NULL)
    {
      break;  // We arrived at the end of the conversion table 
    }
    else
    {
      sscanf(new_line, "%d         %s      %d   %f", &organ_id, organ_name, &mc_material, &density);
      table[organ_id].mc_material = mc_material;
      table[organ_id].density = density; 
    }
  }
     
  fclose(file_conversion);    // Close the file after finding the relevant data (it will be re-opened next iteration of the loop)

      
      
  // -- Open the raw data file and create the output file (with '.vox' appended):
  FILE* file_raw_data = fopen(argv[2], "rb");
  FILE* file_output   = fopen(strcat(argv[2],".vox"), "w");



  // -- Read the raw data inverting the Z axis:
  int *phantom = (int*) malloc(num_voxels_x*num_voxels_y*num_voxels_z*(int)sizeof(int));
  unsigned char read_voxel_value;
  int i, j, k, int_dummy, voxel_value;
  
  for(k=num_voxels_z-1; k>=0; k--)  // inverting the Z axis
  {
    for(j=0; j<num_voxels_y; j++)
    {
      for(i=0; i<num_voxels_x; i++)
      {
        register int voxel = i + j*num_voxels_x + k*num_voxels_y*num_voxels_x;        
        int_dummy = fread(&read_voxel_value, sizeof(unsigned char), 1, file_raw_data);
        phantom[voxel] = (int)read_voxel_value;
      }
    }
  }
        
  // -- Write the .vox file header:
  fprintf(file_output, "# >>>> MC-GPU / penEasy 2008 VOXEL GEOMETRY FILE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  fprintf(file_output, "#      Geometry imported from the Zubal phantom (file:  \'%s\')\n", argv[2]);
  fprintf(file_output, "#      See info in: http://noodle.med.yale.edu/zubal/ \n");
  fprintf(file_output, "#\n");
  fprintf(file_output, "[SECTION VOXELS HEADER v.2008-04-13]\n");
  fprintf(file_output, " %4d %4d %4d     No. OF VOXELS IN X,Y,Z\n", num_voxels_x, num_voxels_y, num_voxels_z);
  fprintf(file_output, " %.2lf %.2lf %.2lf     VOXEL SIZE (cm) ALONG X,Y,Z\n", size_voxels_x, size_voxels_y, size_voxels_z);
  fprintf(file_output, " 1                  COLUMN NUMBER WHERE MATERIAL ID IS LOCATED\n");
  fprintf(file_output, " 2                  COLUMN NUMBER WHERE THE MASS DENSITY IS LOCATED\n");
  fprintf(file_output, " 1                  BLANK LINES AT END OF X,Y-CYCLES (1=YES,0=NO)\n");
  fprintf(file_output, "[END OF VXH SECTION]\n");
  

  // -- Write the output file in the order expected for penEasy (x++,y+,z), with blanks between rows and double blanks between planes. Three columns are written:  1) mc_material, 2) density, 3) organ_number (optional column, may help when defining more materials or computing organ doses).  
  for(k=0; k<num_voxels_z; k++)
  {
    for(j=0; j<num_voxels_y; j++)
    {
      for(i=0; i<num_voxels_x; i++)
      {
        register int voxel = i + j*num_voxels_x + k*num_voxels_y*num_voxels_x;
        voxel_value = phantom[voxel];

        if (table[voxel_value].mc_material==1)
          // Write air density using 6 decimals (assuming air is material 1)   !!DeBuG!!
          fprintf(file_output, "%d %1.4lf %d\n", table[voxel_value].mc_material, table[voxel_value].density, (int)voxel_value);
        else
          // Use 3 decimals for all other densities:            
          fprintf(file_output, "%d %.3f %d\n", table[voxel_value].mc_material, table[voxel_value].density, (int)voxel_value);
        
        if (table[voxel_value].mc_material<1)
        {
          printf("\n\n  !!ERROR!! Found a voxel value not found in the conversion table: voxel_value = %d\n\n", (int)voxel_value);
          exit(-1);
        }
          
      }
      fprintf(file_output, "\n");
    }
    fprintf(file_output, "\n");
  }

  fprintf(file_output, "# >>>> END OF VOXEL FILE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
  
  
  free(phantom);
  fclose(file_raw_data);
  fclose(file_output);
  
  printf("\n\a                ==>  Conversion happily finished! :-)  <== \a \n\n");
  return 0;
}


