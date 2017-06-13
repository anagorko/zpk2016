#library(devtols)
#install_github("Rcpp","RcppCore")
#install.packages("pacman)
library(pacman)

p_load(Rcpp,
       mice,
       tidyverse,
       broom,
       ggthemes)

set.seed(1234)

sourceCpp("C:/Users/user/Desktop/Imputations/Imput/corrData.cpp")
sourceCpp("C:/Users/user/Desktop/Imputations/Imput/miceFast.cpp")

#parameters

power = 6 # power of 10 - number of observations - should be adjusted to a computer capabilities

nr_var = 7 #CHANGE - only if you generate a bigger corr matrix:  number of variables - independent and one dependent

grs = 100 # grouping variable - number of groups

iters = 100 # number of iterations for benchmarking

## generete example - data

##positive-defined correlation matrix

cors = matrix(c(1,0.6,0.7,0.4,0.4,0.5,0.35,
                NA,1,0.2,0.05,0.1,0.12,0.15,
                NA,NA,1,0.15,0.15,0.1,0.08,
                NA,NA,NA,1,0.12,0.15,0.1,
                NA,NA,NA,NA,1,0.15,0.2,
                NA,NA,NA,NA,NA,1,0.15,
                NA,NA,NA,NA,NA,NA,1),7,7,byrow = T)

cors[lower.tri(cors)] = t(cors)[lower.tri(cors)]

# automatic corr matrix - close to diagonal

#cors = stats::rWishart(100,nr_var+10,diag(nr_var)) 

#cors = apply(cors,1:2,mean)/(nr_var+10)

#cors

##

model = new(corrData,10,10^power,rep(0,nr_var),cors)

data_bin = model$fill("binom")
data_disc = model$fill("discrete")
data_con = model$fill("contin")

colnames(data_bin) = c("y",paste0("x",1:(nr_var-2)),"group")
colnames(data_disc) = c("y",paste0("x",1:(nr_var-2)),"group")
colnames(data_con) = c("y",paste0("x",1:(nr_var-2)),"group")

## NA index

index_NA = 1:nrow(data_con) %in% sample(1:nrow(data_con),10^(power-1))

######################Discrete

mice.impute.lda = mice.impute.lda(data_disc[,1],!index_NA,data_disc[,c(2:nr_var)])

model = new(miceFast,data_disc[,1],data_disc[,c(2:nr_var)],index_NA)
miceFast.impute.lda =  model$impute("lda")

table(miceFast.impute.lda ,data_disc[index_NA,1])
table(as.numeric(mice.impute.lda),data_disc[index_NA,1])

m1 = microbenchmark::microbenchmark(R=mice.impute.lda(data_disc[,1],!index_NA,data_disc[,c(2:nr_var)]),
                               miceFast={
                                 model = new(miceFast,data_disc[,1],data_disc[,c(2:(nr_var-1))],index_NA)
                                 model$impute("lda")
                               },
                               times=iters)

g1 = autoplot(m1)+theme_economist()+ ggtitle("LDA discrete - without grouping")

ggsave("C:/Users/user/Desktop/Imputations/Imput/g1.png",g1)

### grouping variable


g = data_disc[,nr_var]

data_disc[,nr_var] = floor(pnorm(g)*grs)

data_disc = cbind(data_disc,index_NA)

data_disc = data_disc[order(data_disc[,nr_var]),]

gr= data_disc[,nr_var]

index_NA = as.logical(data_disc[,(nr_var+1)])

pred_Rbase = NULL
for(i in unique(data_disc[,nr_var])){
    temp = data_disc[data_disc[,nr_var]==i,]
    pred = mice.impute.lda(as.vector(temp[,1]),!temp[,(nr_var+1)],as.matrix(temp[,c(2:(nr_var-1))]))
    pred_Rbase = c(pred_Rbase,as.numeric(pred))
}

pred_dplyr = data_disc %>%
  as.data.frame() %>%
  group_by(group) %>% 
  do(im = mice.impute.lda(as.matrix(.[,1]),!.$index_NA,as.matrix(.[,c(2:(nr_var-1))]))) %>% 
  tidy(im)  %>% ungroup()%>% select(x) %>% unlist() %>% as.numeric()


model = new(miceFast,data_disc[,1],data_disc[,c(2:(nr_var-1))],index_NA,gr,TRUE)
model$sortby_g()
pred_miceFast =  model$imputeby("lda")

  
table(pred_miceFast,data_disc[index_NA,1])
table(pred_dplyr,data_disc[index_NA,1])
table(pred_Rbase,data_disc[index_NA,1])


##Performance

m2 = microbenchmark::microbenchmark(
  dplyr={
  pred_dplyr = data_disc %>%
    as.data.frame() %>%
    group_by(group) %>% 
    do(im = mice.impute.lda(.$y,!.$index_NA,as.matrix(.[,c(2:(nr_var-1))]))) %>% 
    tidy(im)  %>% 
    ungroup()%>% 
    select(x) %>% 
    unlist() %>% 
    as.numeric()},
  R_base={
      pred_all = NULL
      for(i in unique(data_disc[,nr_var])){
        temp = data_disc[data_disc[,nr_var]==i,]
        pred = mice.impute.lda(as.matrix(temp[,1]),!temp[,(nr_var+1)],as.matrix(temp[,c(2:(nr_var-1))]))
        pred_all = c(pred_all,as.numeric(pred))}},
  miceFast={
    model = new(miceFast,data_disc[,1],data_disc[,c(2:(nr_var-1))],index_NA,data_disc[,nr_var],TRUE)
    pred_miceFast = model$imputeby("lda")},
  times=iters)

g2 = autoplot(m2)+theme_economist()+ ggtitle("LDA discrete - with grouping")

ggsave("C:/Users/user/Desktop/Imputations/Imput/g2.png",g2)


#######################Binom

mice.impute.lda = mice.impute.lda(data_bin[,1],!index_NA,data_bin[,c(2:(nr_var-1))])

model = new(miceFast,data_bin[,1],data_bin[,c(2:(nr_var-1))],index_NA)
miceFast.impute.lda =  model$impute("lda")

table(miceFast.impute.lda ,data_bin[index_NA,1])
table(mice.impute.lda,data_bin[index_NA,1])

m3 = microbenchmark::microbenchmark(R=mice.impute.lda(data_bin[,1],!index_NA,data_bin[,c(2:(nr_var-1))]),
                               miceFast={
                                 model = new(miceFast,data_bin[,1],data_bin[,c(2:(nr_var-1))],index_NA)
                                 model$impute("lda")
                               },
                               times=iters)

g3 = autoplot(m3)+theme_economist()+ ggtitle("LDA binom - without grouping")

ggsave("C:/Users/user/Desktop/Imputations/Imput/g3.png",g3)


#####################Continous - LM Noise


mice.impute.norm.nob = mice.impute.norm.nob(data_con[,1],!index_NA,data_con[,c(2:(nr_var-1))])

model = new(miceFast,data_con[,1],data_con[,c(2:(nr_var-1))],index_NA)
miceFast.impute.norm.noise =  model$impute("lm_noise")

sum((miceFast.impute.norm.noise - data_con[index_NA,1])^2)
sum((mice.impute.norm.nob - data_con[index_NA,1])^2)

m4 = microbenchmark::microbenchmark(R = mice.impute.norm.nob(data_con[,1],!index_NA,data_con[,c(2:(nr_var-1))]),
                               miceFast={
                                 model = new(miceFast,data_con[,1],cbind(1,data_con[,c(2:(nr_var-1))]),index_NA)
                                 model$impute("lm_noise")
                               },
                               times=iters)

g4 = autoplot(m4)+theme_economist()+ ggtitle("linear regression noise - without grouping")

ggsave("C:/Users/user/Desktop/Imputations/Imput/g4.png",g4)



#####################Continous - LM Bayes


mice.impute.norm.bayes = mice.impute.norm(data_con[,1],!index_NA,data_con[,c(2:(nr_var-1))])

model = new(miceFast,data_con[,1],data_con[,c(2:(nr_var-1))],index_NA)
miceFast.impute.norm.bayes =  model$impute("lm_bayes")

sum((miceFast.impute.norm.bayes - data_con[index_NA,1])^2)
sum((mice.impute.norm.bayes - data_con[index_NA,1])^2)

m5 = microbenchmark::microbenchmark(R = mice.impute.norm(data_con[,1],!index_NA,data_con[,c(2:(nr_var-1))]),
                               miceFast={
                                 model = new(miceFast,data_con[,1],cbind(1,data_con[,c(2:(nr_var-1))]),index_NA)
                                 model$impute("lm_bayes")
                               },
                               times=iters)

g5 = autoplot(m5)+theme_economist()+ ggtitle("linear regression bayes - without grouping")

ggsave("C:/Users/user/Desktop/Imputations/Imput/g5.png",g5)



#####################Continous - LM Predict


mice.impute.norm.pred = mice.impute.norm.predict(data_con[,1],!index_NA,data_con[,c(2:(nr_var-1))])

model = new(miceFast,data_con[,1],cbind(1,data_con[,c(2:(nr_var-1))]),index_NA)
miceFast.impute.norm.pred =  model$impute("lm_pred")

sum((miceFast.impute.norm.pred - data_con[index_NA,1])^2)
sum((mice.impute.norm.pred - data_con[index_NA,1])^2)

m6 = microbenchmark::microbenchmark(R = {
  mice.impute.norm.predict(data_con[,1],!index_NA,data_con[,c(2:(nr_var-1))])
  }
,
miceFast={
  model = new(miceFast,data_con[,1],cbind(1,data_con[,c(2:(nr_var-1))]),index_NA)
  model$impute("lm_pred")
},times=iters)

g6 = autoplot(m6)+theme_economist()+ ggtitle("linear regression predict - without grouping")

ggsave("C:/Users/user/Desktop/Imputations/Imput/g6.png",g6)


## grouping variable

g = data_con[,nr_var]

data_con[,nr_var] = floor(pnorm(g)*grs)

data_con = cbind(data_con,index_NA)

data_con = data_con[order(data_con[,nr_var]),]

gr= data_con[,nr_var]

index_NA = as.logical(data_con[,(nr_var+1)])

pred_Rbase = NULL
for(i in unique(data_con[,nr_var])){
  temp = data_con[data_con[,nr_var]==i,]
  pred = mice.impute.norm.predict(as.matrix(temp[,1]),!temp[,(nr_var+1)],as.matrix(temp[,c(2:(nr_var-1))]))
  pred_Rbase = c(pred_Rbase,pred)
}

pred_dplyr = data_con %>%
    as.data.frame() %>%
    group_by(group) %>% 
    do(im = mice.impute.norm.predict(as.matrix(.[,1]),!.$index_NA,as.matrix(.[,c(2:(nr_var-1))]))) %>%
    tidy(im)  %>% ungroup()%>% select(x) %>% unlist() %>% as.numeric()

model = new(miceFast,data_con[,1],cbind(1,data_con[,c(2:(nr_var-1))]),index_NA,gr,TRUE)
pred_miceFast =  model$imputeby("lm_pred")

sum((pred_miceFast-data_con[index_NA,1])^2)
sum((pred_dplyr-data_con[index_NA,1])^2)
sum((pred_Rbase-data_con[index_NA,1])^2)

##Performance

m7 = microbenchmark::microbenchmark(
  dplyr={
    pred_dplyr = data_con %>%
      as.data.frame() %>%
      group_by(group) %>% 
      do(im = mice.impute.norm.predict(as.matrix(.[,1]),!.$index_NA,as.matrix(.[,c(2:(nr_var-1))]))) %>%
      tidy(im)  %>% 
      ungroup()%>%
      select(x) %>% 
      unlist() %>%
      as.numeric()
    },
  R_base={
    pred_Rbase = NULL
    for(i in unique(data_con[,nr_var])){
      temp = data_con[data_con[,nr_var]==i,]
      pred = mice.impute.norm.predict(as.matrix(temp[,1]),!temp[,(nr_var+1)],as.matrix(temp[,c(2:(nr_var-1))]))
      pred_Rbase = c(pred_Rbase,pred)
    }},
  miceFast={
    model = new(miceFast,data_con[,1],data_con[,c(2:(nr_var-1))],index_NA,data_con[,nr_var],TRUE)
    pred_miceFast = model$imputeby("lm_pred")},
  times=iters)

g7 = autoplot(m7)+theme_economist()+ ggtitle("linear regression predict - with grouping")

ggsave("C:/Users/user/Desktop/Imputations/Imput/g7.png",g7)
