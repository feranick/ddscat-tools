#####################################################################
#
# RunAv-conv for SERS - plotter (to be used with the c++ runav-conv)
#
# Version 1-20130928
#
# Nicola Ferralis - ferralis@mit.edu
#
# Released under GNU Public License v.3
#
#####################################################################
library(Hmisc);library(akima); library(fields);library(plotrix)

numTicks=12;
dimPlot=7;

inputFile="b_ra-c-r0.482um.csv"
radius = 0.482  #radius of the beam

rootName=gsub(".csv","",inputFile)
outFile<-paste(rootName,"-plots.pdf",sep="")

# Read X, Y data in matrix
t = read.csv(inputFile, header = FALSE, skip = 1)


# Transform into vectors
E4=as.vector(t[,2])
y=as.vector(t[,5])
z=as.vector(t[,6])
runav=as.vector(t[,7])
conv=as.vector(t[,8])


sE4=(log(max(E4))-log(min(E4)))/numTicks
sav=(log(max(runav))-log(min(runav)))/numTicks
#sconv=(log(max(conv))-log(min(conv)))/numTicks
sconv=(max(conv)-min(conv))/numTicks


ticks = min(E4)
ticks2 = min(runav)
ticks3 = min(conv)

for (i in seq(1,numTicks,1)){
	ticks<- c(ticks, log(min(E4))+sE4*i)
	ticks2<- c(ticks2, log(min(runav))+sav*i)
	#ticks3<- c(ticks3, log(min(conv))+sconv*i)
	ticks3<- c(ticks3, min(conv)+sconv*i)
}

pdf(file=outFile, width=dimPlot, height=dimPlot, onefile=T)

ticks<-exp(ticks)
ticks<-round(ticks,digits=1)

image.plot(interp(y,z,log(E4)), axis.args=list( at=format(log(ticks),1), labels=ticks), legend.args=list( text="E4",cex=1.0, side=3, line=1), main=inputFile, xlab="um", ylab="um")

ticks2<-exp(ticks2)
ticks2<-round(ticks2,digits=1)

image.plot(interp(y,z,log(runav)), axis.args=list( at=log(ticks2), labels=ticks2),legend.args=list( text="Run-Av-E4",cex=1.0, side=3, line=1), main=inputFile, xlab="um", ylab="um")


draw.circle(0,0,radius,nv=100,border=NULL,col=NA,lty=1,lwd=1)


#ticks3<-exp(ticks3)
ticks3<-round(ticks3,digits=1)

#image.plot(interp(y,z,log(conv)), axis.args=list( at=log(ticks3), labels=ticks3),legend.args=list( text="Gauss-conv-E4",cex=1.0, side=3, line=1), main=inputFile, xlab="um", ylab="um")
image.plot(interp(y,z,conv), legend.args=list( text="Gauss-conv-E4",cex=1.0, side=3, line=1), zlim=c(min(conv),max(conv)), main=inputFile, xlab="um", ylab="um")


draw.circle(0,0,radius,nv=100,border=NULL,col=NA,lty=1,lwd=1)


dev.off()