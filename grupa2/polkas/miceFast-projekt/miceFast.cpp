//[[Rcpp::depends(RcppArmadillo)]]
//[[Rcpp::plugins(cpp11)]]


#include <RcppArmadillo.h>
#include <string>
#include <map>

//Building class

class miceFast{

  arma::colvec y;     //dependent
  arma::mat x;       //independent
  arma::uvec lack;  //index of lack
  arma::uvec g;    //grouping
  bool sorted;    //sorted or not
  arma::colvec w;//weights

public:

  miceFast(arma::colvec _y, arma::mat _x,arma::uvec  _lack):
    y(_y),
    x(_x),
    lack(_lack){};

  miceFast(arma::colvec _y,arma::mat _x, arma::uvec  _lack,arma::uvec _g,bool _s):
    y(_y),
    x(_x),
    lack(_lack),
    g(_g),
    sorted(_s){};

  miceFast( arma::colvec _y, arma::mat _x,arma::uvec  _lack,arma::colvec _w):
    y(_y),
    x(_x),
    lack(_lack),
    w(_w){};

  miceFast(arma::colvec _y,arma::mat _x,arma::uvec  _lack,arma::uvec _g,bool _s,arma::colvec _w):
    y(_y),
    x(_x),
    lack(_lack),
    g(_g),
    sorted(_s),
    w(_w){};

  miceFast(){};

  arma::colvec imputeby(std::string s);

  arma::colvec impute(std::string s);

  arma::colvec imputebyW(std::string s);

  arma::colvec imputeW(std::string s);

  std::string get_models();

  void sortData_byg();

  arma::uvec get_index_full();

  arma::uvec get_index_NA();

};

//weighted linear regression

arma::colvec fastLm_weighted(arma::colvec &y, arma::mat &X,arma::colvec &w, arma::mat &X1) {

  int n = X.n_rows, k = X.n_cols;

  arma::colvec wq=sqrt(w);
  arma::colvec y2 = wq%y;
  arma::mat X2(n,k);

  for(int h=0;h<k;h++){
    X2.col(h)=wq%X.col(h);
  }

  arma::colvec coef = arma::solve(X2, y2);

  return X1*coef;

}

//weighted linear regression - noise

arma::colvec fastLm_weighted_noise(arma::colvec &y, arma::mat &X,arma::colvec &w, arma::mat &X1) {

  int N = X.n_rows; int C = X.n_cols; int N_NA = X1.n_rows;

  arma::colvec wq=sqrt(w);
  arma::colvec y2 = wq%y;
  arma::mat X2(N,C);

  for(int h=0;h<C;h++){
    X2.col(h)=wq%X.col(h);
  }

  arma::colvec coef = arma::solve(X2, y2);

  arma::colvec res = y - X*coef;

  double sigma = sqrt(arma::as_scalar(arma::trans(res)*res)/(N - C - 1));

  arma::vec noise(N_NA);
  noise.randn();

  return X1 * coef + noise * sigma;

}

//weighted linear regression - bayes

arma::colvec fastLm_weighted_bayes(arma::colvec &y, arma::mat &X,arma::colvec &w, arma::mat &X1) {

  int N = X.n_rows; int C = X.n_cols; int N_NA = X1.n_rows;

  arma::colvec wq=sqrt(w);
  arma::colvec y2 = wq%y;
  arma::mat X2(N,C);

  for(int h=0;h<C;h++){
    X2.col(h)=wq%X.col(h);
  }

  arma::colvec coef = arma::solve(X2, y2);

  arma::colvec res = y - X*coef;

  double df = N-C ;

  arma::vec chi2 = Rcpp::rchisq(1, df);

  double sigma_b = sqrt(arma::as_scalar(arma::trans(res)*res)/arma::as_scalar(chi2));

  arma::vec noise(C);
  noise.randn();

  arma::mat XX_inv = arma::inv(arma::trans(X)*X) ;

  arma::colvec beta_b = coef + arma::trans(arma::chol(XX_inv)) * noise * sigma_b;

  arma::vec noise2(N_NA);
  noise2.randn();

  return X1 * coef + noise2 * sigma_b;

}

//linear regression - bayes

arma::colvec fastLm_bayes(arma::colvec &y, arma::mat &X,  arma::mat &X1) {

  int N = X.n_rows; int C = X.n_cols; int N_NA = X1.n_rows;

  arma::colvec coef = arma::solve(X,y,arma::solve_opts::fast);//arma::inv(X.t()*X)*X.t()*y;

  arma::colvec res = y - X*coef;

  double df = N-C ;

  arma::vec chi2 = Rcpp::rchisq(1, df);

  double sigma_b = sqrt(arma::as_scalar(arma::trans(res)*res)/arma::as_scalar(chi2));

  arma::vec noise(C);
  noise.randn();

  arma::mat XX_inv = arma::inv(arma::trans(X)*X) ;

  arma::colvec beta_b = coef + arma::trans(arma::chol(XX_inv)) * noise * sigma_b;

  arma::vec noise2(N_NA);
  noise2.randn();

  return X1 * coef + noise2 * sigma_b;

}

//Linear regression with noise

arma::colvec fastLm_noise(arma::colvec &y,arma::mat &X,arma::mat &X1) {

  int N = X.n_rows; int C = X.n_cols; int N_NA = X1.n_rows;

  arma::colvec coef = arma::solve(X,y,arma::solve_opts::fast);//arma::inv(X.t()*X)*X.t()*y;

  arma::colvec res = y - X*coef;

  double sigma = sqrt(arma::as_scalar(arma::trans(res)*res)/(N - C - 1));

  arma::vec noise(N_NA);
  noise.randn();

  return X1 * coef + noise * sigma;

}

//Simple linear regression

arma::colvec fastLm_pred(arma::colvec &y, arma::mat &X, arma::mat &X1) {

  arma::colvec coef = arma::solve(X, y,arma::solve_opts::fast);//arma::inv(X.t()*X)*X.t()*y;

  return X1*coef;

}

//LDA prediction model

arma::colvec fastLda( arma::colvec &y,  arma::mat &X,arma::mat &X_NA) {

  double tol = 1e-4;

  int N = X.n_rows; int C = X.n_cols;

  arma::vec un = arma::unique(y);

  int group = un.n_elem;

  if(group==1 || group >30){
    Rcpp::stop("minimum 2 and maximum 30 categories");
  }

  arma::vec counts = arma::conv_to<arma::vec>::from(arma::hist(y,un));

  arma::vec prior = counts/ (double) N;

  arma::mat group_means(group,C);

  for(int i=0;i<group;i++){

    arma::uvec index = arma::find(y == un(i));
    group_means.row(i) = arma::mean(X.rows(index),0);

  }

  arma::mat group_means_mat(N,C);

  for(int i=0;i<N;i++) group_means_mat.row(i) = group_means.rows(arma::find(un == y(i)));

  arma::mat Sw = arma::sqrt(arma::cov(X - group_means_mat));

  arma::vec scaling0 = 1/Sw.diag();

  arma::mat scaling = arma::diagmat(scaling0);

  double fac =  1/(double)N;

  arma::mat X0 = sqrt(fac) * (X - group_means_mat) * scaling;

  arma::mat input = X0.t()*X0;
  arma::mat U;
  arma::mat V;
  arma::vec s;

  arma::svd( U, s, V, input );

  arma::uvec proper = arma::find(s > tol);

  int rank = proper.n_elem;

  if(rank == 0){
    Rcpp::stop("rank = 0: variables are numerically constant");
  }

  if(rank < C){
    Rcpp::warning("variables are collinear");
  }

  scaling = scaling * U.cols(proper)  * arma::diagmat(1/s.elem(proper));

  arma::vec xb = group_means.t()*prior;

  fac = 1/(double) C;

  X0 = arma::conv_to<arma::rowvec>::from(sqrt(((double) N * prior)*fac))* (group_means.each_row() - xb.t())  * scaling;

  arma::svd( U, s, V, X0.t()*X0 );

  proper = arma::find(s > tol * s(0));

  rank = proper.n_elem;

  if(rank == 0){
    Rcpp::stop("group means are numerically identical");
  }

  scaling = scaling * U.cols(proper);

  arma::mat X_means = arma::mean(X,0);

  arma::vec dist_base = 0.5 * arma::mean(arma::pow(group_means * scaling,2),1) - arma::log(prior);

  N = X_NA.n_rows;

  //Prediction

  arma::mat dist_base_mat =  arma::repmat(arma::trans(dist_base),N,1);

  arma::mat dist_raw = dist_base_mat - X_NA * scaling * arma::trans(group_means * scaling);

  arma::mat dist_scale = arma::exp(-(dist_raw - arma::repmat(arma::min(dist_raw,1),1,group)));

  arma::colvec pred(N);

  for(int i =0;i<N;i++){
    pred(i) = un(arma::index_max(dist_scale.row(i)));
  }

  return pred;


}

//functions for getting indexes

arma::uvec miceFast::get_index_full(){

  arma::uvec index_Full = arma::find(lack == 0);

  return index_Full;
}

arma::uvec miceFast::get_index_NA(){

  arma::uvec index_Full = arma::find(lack == 1);

  return index_Full;
}

//function for sorting data by a grouping variable

void miceFast::sortData_byg(){

  arma::uvec order = arma::stable_sort_index(g);

  x = x.rows(order);
  y = y.elem(order);
  if(!w.empty()){w = w.elem(order);}


}

//function for printing recommended prediction models

std::string miceFast::get_models(){

  arma::vec un = arma::unique(y);

  int un_n =  un.n_elem;
  std::string type;

  if(un_n == 2){
    type = "recommended lda or (lm_pred,lm_bayes,lm_noise - round results)";
  }
  else if(un_n > 2 && un_n < 30){
    type = "recommended lda or (lm_pred,lm_bayes,lm_noise - remember to round results if needed)";
  }
  else if(un_n > 30){
    type = "lm_pred or lm_bayes or lm_noise";
  }
  else {type = "one unique value";}

  return type;
}

// map - implementing functions

typedef arma::colvec (*pfunc)(arma::colvec&,arma::mat&,arma::mat&);
std::map<std::string, pfunc> funMap = {{"lda",fastLda},
{"lm_pred",fastLm_pred},
{"lm_noise",fastLm_noise},
{"lm_bayes",fastLm_bayes}};

//Impute

arma::colvec miceFast::impute(std::string s){

  arma::mat X_full = x.rows(get_index_full());
  arma::mat X_NA = x.rows(get_index_NA());
  arma::colvec Y_full = y.elem(get_index_full());

  pfunc f = funMap[s];
  arma::colvec pred = (*f)(Y_full,X_full,X_NA);

  return pred;
}

//Impute with grouping

arma::colvec miceFast::imputeby(std::string s){

  if(g.is_empty()){
    return miceFast::impute(s);
  }

  if(sorted == false){
    sortData_byg();
  }

  //grouping variable

  arma::uvec un = arma::unique(g);

  unsigned int group = un.n_elem;

  //quantitative model

  pfunc fun = funMap[s];

  //dividing data to NA and full

  arma::mat X_full = x.rows(get_index_full());
  arma::mat X_NA = x.rows(get_index_NA());
  arma::colvec Y_full = y.rows(get_index_full());


  arma::uvec g_full = g.elem(get_index_full());
  arma::uvec g_NA = g.elem(get_index_NA());

  //predictions container

  arma::colvec pred_all(get_index_NA().n_elem);

  //iter
  unsigned int start = 0;
  unsigned int end = 0;

  for(unsigned int  a=0;a<group;a++){

    unsigned int gg = un(a);

    arma::uvec find_g = arma::find(g_full==gg);
    arma::uvec find_g_NA = arma::find(g_NA==gg);

    arma::mat X_full_0 = X_full.rows(find_g) ;
    arma::mat X_NA_0 = X_NA.rows(find_g_NA);
    arma::colvec Y_full_0 =  Y_full.elem(find_g);

    arma::colvec pred =  (*fun)(Y_full_0,X_full_0,X_NA_0);

    end = start + pred.n_elem-1;

    pred_all(arma::span(start,end)) = pred;

    start = end+1;

  }

  return pred_all;
}

//WEIGHTED

typedef arma::colvec (*pfuncw)(arma::colvec&,arma::mat&,arma::colvec&,arma::mat&);
std::map<std::string, pfuncw> funMapw = {{"lm_pred",fastLm_weighted},
                                         {"lm_noise",fastLm_weighted_noise},
                                         {"lm_bayes",fastLm_weighted_bayes}};

arma::colvec miceFast::imputeW(std::string s){

  if(w.empty()){return miceFast::impute(s);}

  arma::mat X_full = x.rows(get_index_full());
  arma::mat X_NA = x.rows(get_index_NA());
  arma::colvec Y_full = y.elem(get_index_full());
  arma::colvec w_full = w.elem(get_index_full());

  pfuncw f = funMapw[s];
  arma::colvec pred = (*f)(Y_full,X_full,w_full,X_NA);

  return pred;
}

//Impute with grouping

arma::colvec miceFast::imputebyW(std::string s){

  if(w.empty()){return miceFast::imputeby(s);}

  if(g.is_empty()){
    return miceFast::impute(s);
  }

  if(sorted == false){
    sortData_byg();
  }

  //grouping variable

  arma::uvec un = arma::unique(g);

  unsigned int group = un.n_elem;

  //quantitative model

  pfuncw fun = funMapw[s];

  //dividing data to NA and full

  arma::mat X_full = x.rows(get_index_full());
  arma::mat X_NA = x.rows(get_index_NA());
  arma::colvec Y_full = y.rows(get_index_full());
  arma::colvec w_full = w.rows(get_index_full());


  arma::uvec g_full = g.elem(get_index_full());
  arma::uvec g_NA = g.elem(get_index_NA());

  //predictions container

  arma::colvec pred_all(get_index_NA().n_elem);

  //iter
  unsigned int start = 0;
  unsigned int end = 0;

  for(unsigned int  a=0;a<group;a++){

    unsigned int gg = un(a);

    arma::uvec find_g = arma::find(g_full==gg);
    arma::uvec find_g_NA = arma::find(g_NA==gg);

    arma::mat X_full_0 = X_full.rows(find_g) ;
    arma::mat X_NA_0 = X_NA.rows(find_g_NA);
    arma::colvec Y_full_0 =  Y_full.elem(find_g);
    arma::colvec w_full_0 =  w_full.elem(find_g);

    arma::colvec pred =  (*fun)(Y_full_0,X_full_0,w_full_0,X_NA_0);

    end = start + pred.n_elem-1;

    pred_all(arma::span(start,end)) = pred;

    start = end+1;

  }

  return pred_all;
}



RCPP_MODULE(miceFast){
  using namespace Rcpp ;

  class_<miceFast>("miceFast")
    .constructor<arma::colvec,arma::mat,arma::uvec>()
    .constructor<arma::colvec,arma::mat,arma::uvec,arma::uvec,bool>()
    .constructor<arma::colvec,arma::mat,arma::uvec,arma::colvec>()
    .constructor<arma::colvec,arma::mat,arma::uvec,arma::uvec,bool,arma::colvec>()
    .method("impute", &miceFast::impute)
    .method("imputeby", &miceFast::imputeby)
    .method("imputeW", &miceFast::imputeW)
    .method("imputebyW", &miceFast::imputebyW)
    .method("get_models", &miceFast::get_models)

  ;}
