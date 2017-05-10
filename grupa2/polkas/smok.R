 smok = read.csv("C:/Users/user/Desktop/smoka.txt")
 plot(round(smok,3))
 
 
 for(i in 1:nrow(smok)){
   
   plot(smok[1:i,])
   if(i%%100==0) dev.off()
   Sys.sleep(0.1)
   
 }