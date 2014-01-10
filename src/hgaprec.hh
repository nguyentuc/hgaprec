#ifndef HGAPREC_HH
#define HGAPREC_HH

#include "env.hh"
#include "ratings.hh"
#include "gpbase.hh"

class HGAPRec {
public:
  HGAPRec(Env &env, Ratings &ratings);
  ~HGAPRec();

  void vb();
  void vb_bias();
  void vb_hier();
  void gen_ranking_for_users();
  
private:
  void initialize();
  void approx_log_likelihood();
  void get_phi(GPBase<Matrix> &a, uint32_t ai, 
	       GPBase<Matrix> &b, uint32_t bi, 
	       Array &phi);
  void get_phi(GPBase<Matrix> &a, uint32_t ai, 
	       GPBase<Matrix> &b, uint32_t bi, 
	       double biasa, double biasb,
	       Array &phi);
  
  void load_validation_and_test_sets();
  void compute_likelihood(bool validation);
  double pair_likelihood(uint32_t p, uint32_t q, yval_t y) const;
  double log_factorial(uint32_t n)  const;
  
  void do_on_stop();
  void compute_precision(bool save_ranking_file);
  double prediction_score(uint32_t user, uint32_t movie) const;

  void load_beta_and_theta();
  void save_model();

  double rating_likelihood(uint32_t p, uint32_t q, yval_t y) const;
  uint32_t duration() const;

  Env &_env;
  Ratings &_ratings;

  uint32_t _n;
  uint32_t _m;
  uint32_t _k;
  uint32_t _iter;

  GPMatrixGR _theta;
  GPMatrixGR _beta;

  GPMatrixGR _thetabias;
  GPMatrixGR _betabias;

  GPArray _thetarate;
  GPArray _betarate;

  CountMap _validation_map;  
  CountMap _test_map;

  UserMap _sampled_users;
  UserMap _sampled_movies;

  uint32_t _start_time;
  gsl_rng *_r;

  FILE *_hf;
  FILE *_vf;
  FILE *_tf;
  FILE *_af;
  FILE *_pf;

  double _prev_h;
  uint32_t _nh;
  bool _save_ranking_file;
  bool _use_rate_as_score;
  uint32_t _topN_by_user;
};

inline uint32_t
HGAPRec::duration() const
{
  time_t t = time(0);
  return t - _start_time;
}

#endif
