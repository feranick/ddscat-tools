//*****************************************************************************
//
//		randomsphere
//
//		v. 2.0-20130906
//
//		2013 - Nicola Ferralis - ferralis@mit.edu
//
//		Create slabs out of adjacent spheres to be used in DDSCAT
//		in the framework of "Spheres"
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
#include <errno.h>
#include <time.h>
using namespace std;

int operate(char *namein);
void createNew();
double random(double MaxValue);
int rsig();



char version[]="2.0-20130906";
char extension[]="dds.";
char extensiontarg[]=".targ";
char nameout[]="randsphere.txt";
bool randType=true;


int main(int argc, char *argv[])
{
    
    if(argc<2)
	{cout<< "\n randomsphere v."<<version<< "\n\n";
    cout<< " Usage:\n randomsphere <filename> \n";
    cout<<" To create a new source file: randomsphere -n \n\n";}
    
    if(argc>=2)
	{   if(strcmp(argv[1],"-n")==0)
            {createNew();}
        else
           {operate(argv[1]);}

    return 0;
    }
}


//OPERATE 
int operate(char *namein)

{       
    double xLS, yLS, zLS, RSSmin, RSSmax, RLS, numSS, maxExt = 0.0;
    int compLS, compSS, maxRandComp = 1;
    bool targfile, LS= true;
    bool randComp = false;
    
    
    ifstream infile(namein);
	
    if(!infile)
    {cout<<"\n file '"<< namein<<"' not found\n\n";
		return 0;}
        
    string line;
    
    getline(infile, line);
    getline(infile, line);
    targfile=(bool) atof(line.c_str());
    
    getline(infile, line);
    getline(infile, line);
    randType=(bool) atof(line.c_str());
    
    getline(infile, line);
    getline(infile, line);
    LS=(bool) atof(line.c_str());
    
    //for (int n=0; n<numLayer; n++) {
            getline(infile, line);
            getline(infile, line);
            compLS=atof(line.c_str());
        
            getline(infile, line);
            getline(infile, line);
            compSS=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            xLS=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            yLS=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            zLS=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            RLS=atof(line.c_str());

            getline(infile, line);
            getline(infile, line);
            numSS=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            RSSmin=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            RSSmax=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            maxExt=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            randComp=(bool) atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            maxRandComp=atof(line.c_str());
    
    
        if(RSSmax < RSSmin)
        {cout<<"\n RSSmax must be larger than RSSMin \n\n";
            return 0;}
    
    cout<<"\n Creating random spehres from: "<<namein<<"\n";
    
    if(randType==true) {
        cout<<"\n Randomization routine: NO SEED \n";}
    else {
        cout<<"\n Randomization routine: with SEED \n";}
    
    cout<<"\n compLS\tsaveLS\tcompSS\txLS\tyLS\tzLS\tRLS\tRSSmin\tRSSmax\tmaxExt\tnumSS\n";
    
    
    cout<<" "<<compLS<<"\t"<<LS<<"\t";
    if (randComp==false) {
        cout<<compSS<<"\t";
        }
    else {
        cout<<"rand-"<<maxRandComp<<"\t";
        }
    
    cout<<xLS<<"\t"<<yLS<<"\t"<<zLS;
    cout<<"\t"<<RLS<<"\t"<<RSSmin<<"\t"<<RSSmax<<"\t"<<maxExt<<"\t"<<numSS<<"\n";
    
    

    infile.close();
    
    char* outname;
    
    if(targfile==false) {
        outname=strcat(extension,namein);
    }
    
    else {
        char *tempname;
        tempname=(char *)malloc(sizeof(char[100]));
        strncpy(tempname, namein,strlen(namein)-4);
        outname=strcat(tempname,extensiontarg);
        free(tempname);
    }
        
    
    ofstream outfile(outname);
    
    if(targfile==true) {
        if(LS==true) {
            outfile<<"\t\t"<<numSS+1;}
        if(LS==false) {
            outfile<<"\t\t"<<numSS;}
        if(randType==true)
            {outfile<<"\n Random (NO seed) spheres on large sphere (by randomsphere)\n";}
        if(randType==false)
            {outfile<<"\n Random (seed) spheres on large sphere (by randomsphere)\n";}
        outfile<<"  0.390732 -0.920504 -0.000001 = A_1 vector\n";
        outfile<<"  0.920504  0.390732  0.000000 = A_2 vector\n";
        outfile<<"\t x(j)        y(j)        z(j)      a(j)    IC1 IC2 IC3 th,ph,be\n";
        if(LS==true)
            {outfile<<"\t"<<xLS<<"\t"<<yLS<<"\t"<<zLS<<"\t"<<RLS<<"\t"<<compLS<<"\t"<<compLS<<"\t"<<compLS<<"\t0\t0\t0\n";}
    }
    
    //////////////////////////////////////////////////////////
    
    int nSS=0;
    while(nSS <numSS){
        
        double x, y, z, x1, y1, z1, R = 0.0;
        
        x=random(1)*rsig();
        y=random(sqrt(1-pow(x,2)))*rsig();
        z=sqrt(1-pow(x,2)-pow(y,2))*rsig();
        
        x1=(x*RLS+xLS)+maxExt*rsig();
        y1=(y*RLS+xLS)+maxExt*rsig();
        z1=(z*RLS+xLS)+maxExt*rsig();
        R=RSSmin+random(RSSmax-RSSmin);
        

        if (randComp==true) {
            compSS=0;
            while (compSS<1)
                {compSS=random((int) maxRandComp)+1;}
            }
        
        outfile<<"\t"<<x1<<"\t"<<y1<<"\t"<<z1<<"\t"<<R<<"\t"<<compSS<<"\t"<<compSS<<"\t"<<compSS<<"\t0\t0\t0\n";
        nSS++;
        }
    
    //////////////////////////////////////////////////////////
    
    outfile.close();
  
 
    cout<<"\n Saved in: "<<outname;

    if(LS==false)
        {cout<<"\n\n Number of random spheres created: "<<numSS<<"\n\n";}
    if(LS==true)
        {cout<<"\n\n Number of spheres created: "<<numSS+1<<" (center + small sphere)\n\n";}
    
return 0;}
           
           
void createNew(){
    ofstream outfile(nameout);
    outfile<<"# output file: (1: target for ddscat) (0: coordinates only)\n1\n";
    outfile<<"# Randomization routine (1: NO SEED - repeatable) (0: RANDOM)\n1\n";
    outfile<<"# Include center large sphere (1: YES; 0: NO)\n1\n";
    outfile<<"# composition of center sphere\n1\n";
    outfile<<"# composition of small spheres\n1\n";
    outfile<<"# x coordinate (center of the large sphere)\n0\n";
    outfile<<"# y coordinate (center of the large sphere)\n0\n";
    outfile<<"# z coordinate (center of the large sphere)\n0\n";
    outfile<<"# Radius of the large sphere\n1.0\n";
    outfile<<"# number of small spheres\n600\n";
    outfile<<"# Minimum radius of small spheres\n.05\n";
    outfile<<"# Maximum radius of small spheres\n0.2\n";
    outfile<<"# Maximum offset in position of center of small spheres from the surface of large sphere\n0.01\n";
    outfile<<"# Randomize composition of small spheres (1: YES; 0: NO)\n0\n";
    outfile<<"# Max number of random compositions for small spheres\n3\n";
    
    outfile.close();
    cout<<"\n Saved in: "<<nameout<<"\n\n";

}



double random(double MaxValue)
{   if(randType==false)
        {srand(time(0));}
	double b= ((double)rand() / ((double)(RAND_MAX)+(double)(1))) * MaxValue;
	return b;
}

int rsig()
{   
    if(randType==false)
        {srand(time(0));}
    
    
	double b= ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
    if (b<0.5)
        {return -1;}
    else
        return 1;
}
