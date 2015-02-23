//*****************************************************************************
//
//		runavconv
//
//		v. 3.4-20150223
//
//		Copyright (C) 2013 - Nicola Ferralis - ferralis@mit.edu
//
//		From a Paraview csv file, extract and saves in csv a new file
//      with running averages and Gaussian convolution
//
//
//		This program (source code and binaries) is free software;
//		you can redistribute it and/or modify it under the terms of the
//		GNU General Public License as published by the Free Software
//		Foundation, in version 3 of the License.
//
//		This program is distributed in the hope that it will be useful,
//		but WITHOUT ANY WARRANTY; without even the implied warranty of
//		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//		GNU General Public License for more details.
//
//		You can find a complete copy of the GNU General Public License at:
//		http://www.gnu.org/licenses/gpl.txt
//
//******************************************************************************

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>
using namespace std;

#define OPENMP

int operate(char *namein, char *nameinfo);
void createNew();


char version[]="3.4-20150223";
char extension2[]="um.csv";
char nameout[]="info.txt";

//double pi=3.14159265359;
double sqrtPi = 1.77245385091;


char extension1[]="-ragc_r";
char extension3[]="_c";

double const_fwhm=2*sqrt(2*log(2));

int main(int argc, char *argv[])
{
    
    if(argc<2)
	{cout<< "\n\n runav v."<<version<< "\n\n";
        cout<< " Compiled for running average and Gaussian convolution\n\n";
        cout<< " Usage:  runavconv <csv-filename>  <info-filename> \n\n";
        cout<<" To create a new source file: runavconv -n \n\n";}
    
    if(argc==2)
	{   if(strcmp(argv[1],"-n")==0)
            {createNew();}
        else
            {cout<< " Usage:  runavconv <csv-filename>  <info-filename> \n\n";}
    }
    if(argc>2)
        {operate(argv[1],argv[2]);}
    
    return 0;
}


//OPERATE 
int operate(char *namein, char *nameinfo)

{   ifstream infofile(nameinfo);
    double r = 0.0;
    bool crop = 0;
    double ymin, ymax, zmin, zmax = 0.0;
    
    if(!infofile)
    {cout<<"\n file '"<< nameinfo<<"' not found\n\n";
		return 0;}

    string lineinfo;
    
    getline(infofile, lineinfo);
    getline(infofile, lineinfo);
    r= atof(lineinfo.c_str());
    
    getline(infofile, lineinfo);
    getline(infofile, lineinfo);
    crop=(bool) atof(lineinfo.c_str());
    
    getline(infofile, lineinfo);
    getline(infofile, lineinfo);
    ymin= atof(lineinfo.c_str());
    
    getline(infofile, lineinfo);
    getline(infofile, lineinfo);
    ymax= atof(lineinfo.c_str());
    
    getline(infofile, lineinfo);
    getline(infofile, lineinfo);
    zmin= atof(lineinfo.c_str());
    
    getline(infofile, lineinfo);
    getline(infofile, lineinfo);
    zmax= atof(lineinfo.c_str());
    
    if(crop==0) {
        cout << "\n Radius\tCROP\tymin\tymax\tzmin\tzmax\n ";
        cout<<r<<" \tON\t"<<ymin<<"\t"<<ymax<<"\t"<<zmin<<"\t"<<zmax<<endl;
    }
    else {
        cout << "\n Radius: "<<r<<"um\n";
        cout<<"  Crop OFF - Imaging full set\n";
        }
    infofile.close();
    
    ifstream infile(namein);
	
    if(!infile)
    {cout<<"\n file '"<< namein<<"' not found\n\n";
		return 0;}
    cout<<"\n Calculating running average and convolution\n  for a "<<2*r<<" um beam from file: "<<namein<<"\n";
    
    string tmp;
    unsigned long ind = 0;
    while (!infile.eof()) {
        
        getline(infile, tmp);
        ind++;
    }
    infile.close();
    ind=ind-1;

    string line[6];
    //double Int[ind], Comp[ind], x[ind], y[ind], z[ind], E4[ind], average[ind], convolution[ind];
    double *Int = new double[ind];
    double *Comp = new double[ind];
    double *x = new double[ind];
    double *y = new double[ind];
    double *z = new double[ind];
    double *E4 = new double[ind];
    double *average = new double[ind];
    double *convolution = new double[ind];
    
    ifstream infile2(namein);
    
    getline(infile2, line[0],',');
    getline(infile2, line[1],',');
    getline(infile2, line[2],',');
    getline(infile2, line[3],',');
    getline(infile2, line[4],',');
    getline(infile2, line[5]);
    
    
    int i2=0;
    double yminr = 0.0;
    double ymaxr = 0.0;
    double zminr = 0.0;
    double zmaxr = 0.0;
    
    while (!infile2.eof()) {
        
        getline(infile2, tmp,',');
        Int[i2]=atof(tmp.c_str());
        getline(infile2, tmp,',');
        E4[i2]=atof(tmp.c_str());
        getline(infile2, tmp,',');
        Comp[i2]=atof(tmp.c_str());
        getline(infile2, tmp,',');
        x[i2]=atof(tmp.c_str());
        getline(infile2, tmp,',');
        y[i2]=atof(tmp.c_str());
        getline(infile2, tmp);
        z[i2]=atof(tmp.c_str());
        
        if(crop==0) {
            yminr=ymin;
            ymaxr=ymax;
            zminr=zmin;
            zmaxr=zmax;
        }
        else{
            if(y[i2]<=yminr) {
                yminr=y[i2];}
            if(y[i2]>=ymaxr) {
                ymaxr=y[i2];}
            if(z[i2]<=zminr) {
                zminr=z[i2];}
            if(z[i2]>=zmaxr) {
                zmaxr=z[i2];}
            
        }
        
        
        i2++;
    }
    
    infile2.close();
    double c=2*r/const_fwhm;
    
    
#ifdef OPENMP
#pragma omp parallel for
#endif
    
    for (unsigned long k=0; k<(ind-1); k++) {
        double *av = new double;
        double *conv = new double;
        double *enorm = new double;
        double *gauss = new double;
        *av = 0.0;
        *conv = 0.0;
        *enorm = 0.0;
        *gauss = 0.0;
        
        int i3 = 0;
        
        for (unsigned long l=0; l<(ind-2); l++) {
           
            if(y[l]>=yminr && y[l]<=ymaxr && y[k]>=yminr && y[k]<=ymaxr && z[l]>=zminr && z[l]<=zmaxr && z[k]>=zminr && z[k]<=zmaxr){
            if( (pow(y[l]-y[k],2) + pow(z[l]-z[k],2)) <= pow(r,2)) {
                
                *av=*av+E4[l];
                i3++;
                
            }
            *gauss = (1/(c*sqrtPi))*exp(-(pow(y[l]-y[k],2) + pow(z[l]-z[k],2))/(2*pow(c,2)));
            
            *conv= *conv + E4[l]* *gauss;
            *enorm = *enorm + *gauss;
            
            }
        }
        //cout<<*av<<"\t"<<i3<<"\n";
        
        if(i3!=0){
        average[k]=*av/i3;
        convolution[k]=*conv/ *enorm;
        }
        
        
        delete av;
        delete conv;
        delete gauss;
        delete enorm;
    }

    ///// Save the results
    
    char* outname = new char[strlen(namein)+strlen(extension1)+strlen(extension2)];
    
    if(crop==0) {
        sprintf(outname, "%.*s%s%s%g%s", (int)  strlen(namein)-4, namein,extension3,extension1, r,extension2);
        }
    else  {
        sprintf(outname, "%.*s%s%g%s", (int)  strlen(namein)-4, namein,extension1, r,extension2);
    }
        
    
    
    ofstream outfile(outname);
    
    outfile<<line[0]<<","<<line[1]<<","<<line[2]<<","<<line[3]<<","<<line[4]<<","<<line[5]<<",Run-Aver-E4,Conv-Gauss\n";

    for (unsigned long k=0; k<(ind-1); k++) {
        
        if(y[k]>=yminr && y[k]<=ymaxr && z[k]>=zminr && z[k]<=zmaxr){

        
        outfile<<Int[k]<<","<<E4[k]<<","<<Comp[k]<<","<<x[k]<<","<<y[k]<<","<<z[k]<<","<<average[k]<<","<<convolution[k]<<"\n";
        }
    }
    
    outfile.close();
    
    delete[] Int;
    delete[] Comp;
    delete[] x;
    delete[] y;
    delete[] z;
    delete[] E4;
    delete[] average;
    delete[] convolution;
 
    cout<<"\n Saved in: "<<outname;
    delete[] outname;
    
    
    cout<<"\n\n";
return 0;}


void createNew(){
    ofstream outfile(nameout);
    outfile<<"# radius of the projecting beam (0.482 um for 633 nm laser, N.A.-0.8)\n0.482\n";
    outfile<<"# Perform on full grid (1: YES; 0:NO - specify details below)\n1\n";
    outfile<<"# ymin\n-1.5\n";
    outfile<<"# ymax\n1.5\n";
    outfile<<"# zmin\n-1.5\n";
    outfile<<"# zmax\n1.5\n";

    outfile.close();
    cout<<"\n Parameter file saved in: "<<nameout<<"\n\n";
    
}

