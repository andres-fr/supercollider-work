#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "s7.h"


/* -------- j0 -------- */
static s7_pointer s7_dl_j0(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_j0_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_j0_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "j0", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)j0(s7_dl_j0_0)));
}
static s7_double j0_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(j0(x));}
static s7_rf_t j0_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(j0_rf_r); return(NULL);}


/* -------- j1 -------- */
static s7_pointer s7_dl_j1(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_j1_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_j1_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "j1", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)j1(s7_dl_j1_0)));
}
static s7_double j1_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(j1(x));}
static s7_rf_t j1_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(j1_rf_r); return(NULL);}


/* -------- erf -------- */
static s7_pointer s7_dl_erf(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_erf_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_erf_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "erf", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)erf(s7_dl_erf_0)));
}
static s7_double erf_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(erf(x));}
static s7_rf_t erf_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(erf_rf_r); return(NULL);}


/* -------- erfc -------- */
static s7_pointer s7_dl_erfc(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_erfc_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_erfc_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "erfc", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)erfc(s7_dl_erfc_0)));
}
static s7_double erfc_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(erfc(x));}
static s7_rf_t erfc_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(erfc_rf_r); return(NULL);}


/* -------- lgamma -------- */
static s7_pointer s7_dl_lgamma(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_lgamma_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_lgamma_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "lgamma", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)lgamma(s7_dl_lgamma_0)));
}
static s7_double lgamma_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(lgamma(x));}
static s7_rf_t lgamma_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(lgamma_rf_r); return(NULL);}


/* -------- fabs -------- */
static s7_pointer s7_dl_fabs(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_fabs_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_fabs_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "fabs", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)fabs(s7_dl_fabs_0)));
}
static s7_double fabs_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(fabs(x));}
static s7_rf_t fabs_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(fabs_rf_r); return(NULL);}


/* -------- ceil -------- */
static s7_pointer s7_dl_ceil(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_ceil_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_ceil_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "ceil", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)ceil(s7_dl_ceil_0)));
}
static s7_double ceil_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(ceil(x));}
static s7_rf_t ceil_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(ceil_rf_r); return(NULL);}


/* -------- nearbyint -------- */
static s7_pointer s7_dl_nearbyint(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_nearbyint_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_nearbyint_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "nearbyint", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)nearbyint(s7_dl_nearbyint_0)));
}
static s7_double nearbyint_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(nearbyint(x));}
static s7_rf_t nearbyint_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(nearbyint_rf_r); return(NULL);}


/* -------- scalbln -------- */
static s7_pointer s7_dl_scalbln(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_scalbln_0;
  int s7_dl_scalbln_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_scalbln_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "scalbln", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_integer(s7_car(arg)))
    s7_dl_scalbln_1 = (int)s7_integer(s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "scalbln", 2, s7_car(arg), "integer"));
  return(s7_make_real(sc, (s7_double)scalbln(s7_dl_scalbln_0, s7_dl_scalbln_1)));
}


/* -------- fma -------- */
static s7_pointer s7_dl_fma(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_fma_0;
  double s7_dl_fma_1;
  double s7_dl_fma_2;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_fma_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "fma", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_fma_1 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "fma", 2, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_fma_2 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "fma", 3, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)fma(s7_dl_fma_0, s7_dl_fma_1, s7_dl_fma_2)));
}


/* -------- rint -------- */
static s7_pointer s7_dl_rint(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_rint_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_rint_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "rint", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)rint(s7_dl_rint_0)));
}
static s7_double rint_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(rint(x));}
static s7_rf_t rint_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(rint_rf_r); return(NULL);}


/* -------- llrint -------- */
static s7_pointer s7_dl_llrint(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_llrint_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_llrint_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "llrint", 0, s7_car(arg), "real"));
  return(s7_make_integer(sc, (s7_int)llrint(s7_dl_llrint_0)));
}
static s7_int llrint_if_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(llrint(x));}
static s7_if_t llrint_if(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_if(sc, s7_cadr(expr))) return(llrint_if_r); return(NULL);}


/* -------- llround -------- */
static s7_pointer s7_dl_llround(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_llround_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_llround_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "llround", 0, s7_car(arg), "real"));
  return(s7_make_integer(sc, (s7_int)llround(s7_dl_llround_0)));
}
static s7_int llround_if_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(llround(x));}
static s7_if_t llround_if(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_if(sc, s7_cadr(expr))) return(llround_if_r); return(NULL);}


/* -------- trunc -------- */
static s7_pointer s7_dl_trunc(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_trunc_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_trunc_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "trunc", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)trunc(s7_dl_trunc_0)));
}
static s7_double trunc_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(trunc(x));}
static s7_rf_t trunc_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(trunc_rf_r); return(NULL);}


/* -------- fmod -------- */
static s7_pointer s7_dl_fmod(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_fmod_0;
  double s7_dl_fmod_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_fmod_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "fmod", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_fmod_1 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "fmod", 2, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)fmod(s7_dl_fmod_0, s7_dl_fmod_1)));
}
static s7_double fmod_rf_r(s7_scheme *sc, s7_pointer **p)
  {s7_rf_t f; s7_double x, y; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); f = (s7_rf_t)(**p); (*p)++; y = f(sc, p); return(fmod(x, y));}
static s7_rf_t fmod_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(fmod_rf_r); return(NULL);}


/* -------- ldexp -------- */
static s7_pointer s7_dl_ldexp(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_ldexp_0;
  int s7_dl_ldexp_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_ldexp_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "ldexp", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_integer(s7_car(arg)))
    s7_dl_ldexp_1 = (int)s7_integer(s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "ldexp", 2, s7_car(arg), "integer"));
  return(s7_make_real(sc, (s7_double)ldexp(s7_dl_ldexp_0, s7_dl_ldexp_1)));
}


/* -------- scalbn -------- */
static s7_pointer s7_dl_scalbn(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_scalbn_0;
  int s7_dl_scalbn_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_scalbn_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "scalbn", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_integer(s7_car(arg)))
    s7_dl_scalbn_1 = (int)s7_integer(s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "scalbn", 2, s7_car(arg), "integer"));
  return(s7_make_real(sc, (s7_double)scalbn(s7_dl_scalbn_0, s7_dl_scalbn_1)));
}


/* -------- exp2 -------- */
static s7_pointer s7_dl_exp2(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_exp2_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_exp2_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "exp2", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)exp2(s7_dl_exp2_0)));
}
static s7_double exp2_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(exp2(x));}
static s7_rf_t exp2_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(exp2_rf_r); return(NULL);}


/* -------- expm1 -------- */
static s7_pointer s7_dl_expm1(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_expm1_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_expm1_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "expm1", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)expm1(s7_dl_expm1_0)));
}
static s7_double expm1_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(expm1(x));}
static s7_rf_t expm1_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(expm1_rf_r); return(NULL);}


/* -------- log10 -------- */
static s7_pointer s7_dl_log10(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_log10_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_log10_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "log10", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)log10(s7_dl_log10_0)));
}
static s7_double log10_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(log10(x));}
static s7_rf_t log10_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(log10_rf_r); return(NULL);}


/* -------- log1p -------- */
static s7_pointer s7_dl_log1p(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_log1p_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_log1p_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "log1p", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)log1p(s7_dl_log1p_0)));
}
static s7_double log1p_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(log1p(x));}
static s7_rf_t log1p_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(log1p_rf_r); return(NULL);}


/* -------- log2 -------- */
static s7_pointer s7_dl_log2(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_log2_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_log2_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "log2", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)log2(s7_dl_log2_0)));
}
static s7_double log2_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(log2(x));}
static s7_rf_t log2_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(log2_rf_r); return(NULL);}


/* -------- ilogb -------- */
static s7_pointer s7_dl_ilogb(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_ilogb_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_ilogb_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "ilogb", 0, s7_car(arg), "real"));
  return(s7_make_integer(sc, (s7_int)ilogb(s7_dl_ilogb_0)));
}
static s7_int ilogb_if_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(ilogb(x));}
static s7_if_t ilogb_if(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_if(sc, s7_cadr(expr))) return(ilogb_if_r); return(NULL);}


/* -------- cbrt -------- */
static s7_pointer s7_dl_cbrt(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_cbrt_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_cbrt_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "cbrt", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)cbrt(s7_dl_cbrt_0)));
}
static s7_double cbrt_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(cbrt(x));}
static s7_rf_t cbrt_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(cbrt_rf_r); return(NULL);}


/* -------- hypot -------- */
static s7_pointer s7_dl_hypot(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_hypot_0;
  double s7_dl_hypot_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_hypot_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "hypot", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_hypot_1 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "hypot", 2, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)hypot(s7_dl_hypot_0, s7_dl_hypot_1)));
}
static s7_double hypot_rf_r(s7_scheme *sc, s7_pointer **p)
  {s7_rf_t f; s7_double x, y; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); f = (s7_rf_t)(**p); (*p)++; y = f(sc, p); return(hypot(x, y));}
static s7_rf_t hypot_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(hypot_rf_r); return(NULL);}


/* -------- pow -------- */
static s7_pointer s7_dl_pow(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_pow_0;
  double s7_dl_pow_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_pow_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "pow", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_pow_1 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "pow", 2, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)pow(s7_dl_pow_0, s7_dl_pow_1)));
}
static s7_double pow_rf_r(s7_scheme *sc, s7_pointer **p)
  {s7_rf_t f; s7_double x, y; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); f = (s7_rf_t)(**p); (*p)++; y = f(sc, p); return(pow(x, y));}
static s7_rf_t pow_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(pow_rf_r); return(NULL);}


/* -------- fdim -------- */
static s7_pointer s7_dl_fdim(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_fdim_0;
  double s7_dl_fdim_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_fdim_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "fdim", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_fdim_1 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "fdim", 2, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)fdim(s7_dl_fdim_0, s7_dl_fdim_1)));
}
static s7_double fdim_rf_r(s7_scheme *sc, s7_pointer **p)
  {s7_rf_t f; s7_double x, y; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); f = (s7_rf_t)(**p); (*p)++; y = f(sc, p); return(fdim(x, y));}
static s7_rf_t fdim_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(fdim_rf_r); return(NULL);}


/* -------- tgamma -------- */
static s7_pointer s7_dl_tgamma(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_tgamma_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_tgamma_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "tgamma", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)tgamma(s7_dl_tgamma_0)));
}
static s7_double tgamma_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(tgamma(x));}
static s7_rf_t tgamma_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(tgamma_rf_r); return(NULL);}


/* -------- copysign -------- */
static s7_pointer s7_dl_copysign(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_copysign_0;
  double s7_dl_copysign_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_copysign_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "copysign", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_copysign_1 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "copysign", 2, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)copysign(s7_dl_copysign_0, s7_dl_copysign_1)));
}
static s7_double copysign_rf_r(s7_scheme *sc, s7_pointer **p)
  {s7_rf_t f; s7_double x, y; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); f = (s7_rf_t)(**p); (*p)++; y = f(sc, p); return(copysign(x, y));}
static s7_rf_t copysign_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(copysign_rf_r); return(NULL);}


/* -------- nan -------- */
static s7_pointer s7_dl_nan(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  char* s7_dl_nan_0;
  arg = args;
  if (s7_is_string(s7_car(arg)))
    s7_dl_nan_0 = (char*)s7_string(s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "nan", 0, s7_car(arg), "string"));
  return(s7_make_real(sc, (s7_double)nan(s7_dl_nan_0)));
}


/* -------- nextafter -------- */
static s7_pointer s7_dl_nextafter(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_nextafter_0;
  double s7_dl_nextafter_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_nextafter_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "nextafter", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_nextafter_1 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "nextafter", 2, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)nextafter(s7_dl_nextafter_0, s7_dl_nextafter_1)));
}
static s7_double nextafter_rf_r(s7_scheme *sc, s7_pointer **p)
  {s7_rf_t f; s7_double x, y; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); f = (s7_rf_t)(**p); (*p)++; y = f(sc, p); return(nextafter(x, y));}
static s7_rf_t nextafter_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(nextafter_rf_r); return(NULL);}


/* -------- nexttoward -------- */
static s7_pointer s7_dl_nexttoward(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_nexttoward_0;
  double s7_dl_nexttoward_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_nexttoward_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "nexttoward", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_nexttoward_1 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "nexttoward", 2, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)nexttoward(s7_dl_nexttoward_0, s7_dl_nexttoward_1)));
}
static s7_double nexttoward_rf_r(s7_scheme *sc, s7_pointer **p)
  {s7_rf_t f; s7_double x, y; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); f = (s7_rf_t)(**p); (*p)++; y = f(sc, p); return(nexttoward(x, y));}
static s7_rf_t nexttoward_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(nexttoward_rf_r); return(NULL);}


/* -------- fpclassify -------- */
static s7_pointer s7_dl_fpclassify(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_fpclassify_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_fpclassify_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "fpclassify", 0, s7_car(arg), "real"));
  return(s7_make_integer(sc, (s7_int)fpclassify(s7_dl_fpclassify_0)));
}
static s7_int fpclassify_if_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(fpclassify(x));}
static s7_if_t fpclassify_if(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_if(sc, s7_cadr(expr))) return(fpclassify_if_r); return(NULL);}


/* -------- isfinite -------- */
static s7_pointer s7_dl_isfinite(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_isfinite_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_isfinite_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "isfinite", 0, s7_car(arg), "real"));
  return(s7_make_integer(sc, (s7_int)isfinite(s7_dl_isfinite_0)));
}
static s7_int isfinite_if_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(isfinite(x));}
static s7_if_t isfinite_if(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_if(sc, s7_cadr(expr))) return(isfinite_if_r); return(NULL);}


/* -------- isinf -------- */
static s7_pointer s7_dl_isinf(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_isinf_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_isinf_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "isinf", 0, s7_car(arg), "real"));
  return(s7_make_integer(sc, (s7_int)isinf(s7_dl_isinf_0)));
}
static s7_int isinf_if_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(isinf(x));}
static s7_if_t isinf_if(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_if(sc, s7_cadr(expr))) return(isinf_if_r); return(NULL);}


/* -------- isnan -------- */
static s7_pointer s7_dl_isnan(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_isnan_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_isnan_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "isnan", 0, s7_car(arg), "real"));
  return(s7_make_integer(sc, (s7_int)isnan(s7_dl_isnan_0)));
}
static s7_int isnan_if_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(isnan(x));}
static s7_if_t isnan_if(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_if(sc, s7_cadr(expr))) return(isnan_if_r); return(NULL);}


/* -------- isnormal -------- */
static s7_pointer s7_dl_isnormal(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_isnormal_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_isnormal_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "isnormal", 0, s7_car(arg), "real"));
  return(s7_make_integer(sc, (s7_int)isnormal(s7_dl_isnormal_0)));
}
static s7_int isnormal_if_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(isnormal(x));}
static s7_if_t isnormal_if(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_if(sc, s7_cadr(expr))) return(isnormal_if_r); return(NULL);}


/* -------- signbit -------- */
static s7_pointer s7_dl_signbit(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_signbit_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_signbit_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "signbit", 0, s7_car(arg), "real"));
  return(s7_make_integer(sc, (s7_int)signbit(s7_dl_signbit_0)));
}
static s7_int signbit_if_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(signbit(x));}
static s7_if_t signbit_if(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_if(sc, s7_cadr(expr))) return(signbit_if_r); return(NULL);}


/* -------- floor -------- */
static s7_pointer s7_dl_floor(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_floor_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_floor_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "floor", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)floor(s7_dl_floor_0)));
}
static s7_double floor_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(floor(x));}
static s7_rf_t floor_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(floor_rf_r); return(NULL);}


/* -------- round -------- */
static s7_pointer s7_dl_round(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_round_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_round_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "round", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)round(s7_dl_round_0)));
}
static s7_double round_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(round(x));}
static s7_rf_t round_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(round_rf_r); return(NULL);}


/* -------- remainder -------- */
static s7_pointer s7_dl_remainder(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_remainder_0;
  double s7_dl_remainder_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_remainder_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "remainder", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_remainder_1 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "remainder", 2, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)remainder(s7_dl_remainder_0, s7_dl_remainder_1)));
}
static s7_double remainder_rf_r(s7_scheme *sc, s7_pointer **p)
  {s7_rf_t f; s7_double x, y; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); f = (s7_rf_t)(**p); (*p)++; y = f(sc, p); return(remainder(x, y));}
static s7_rf_t remainder_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(remainder_rf_r); return(NULL);}


/* -------- exp -------- */
static s7_pointer s7_dl_exp(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_exp_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_exp_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "exp", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)exp(s7_dl_exp_0)));
}
static s7_double exp_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(exp(x));}
static s7_rf_t exp_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(exp_rf_r); return(NULL);}


/* -------- log -------- */
static s7_pointer s7_dl_log(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_log_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_log_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "log", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)log(s7_dl_log_0)));
}
static s7_double log_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(log(x));}
static s7_rf_t log_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(log_rf_r); return(NULL);}


/* -------- sqrt -------- */
static s7_pointer s7_dl_sqrt(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_sqrt_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_sqrt_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "sqrt", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)sqrt(s7_dl_sqrt_0)));
}
static s7_double sqrt_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(sqrt(x));}
static s7_rf_t sqrt_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(sqrt_rf_r); return(NULL);}


/* -------- cos -------- */
static s7_pointer s7_dl_cos(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_cos_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_cos_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "cos", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)cos(s7_dl_cos_0)));
}
static s7_double cos_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(cos(x));}
static s7_rf_t cos_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(cos_rf_r); return(NULL);}


/* -------- sin -------- */
static s7_pointer s7_dl_sin(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_sin_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_sin_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "sin", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)sin(s7_dl_sin_0)));
}
static s7_double sin_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(sin(x));}
static s7_rf_t sin_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(sin_rf_r); return(NULL);}


/* -------- tan -------- */
static s7_pointer s7_dl_tan(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_tan_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_tan_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "tan", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)tan(s7_dl_tan_0)));
}
static s7_double tan_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(tan(x));}
static s7_rf_t tan_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(tan_rf_r); return(NULL);}


/* -------- cosh -------- */
static s7_pointer s7_dl_cosh(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_cosh_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_cosh_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "cosh", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)cosh(s7_dl_cosh_0)));
}
static s7_double cosh_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(cosh(x));}
static s7_rf_t cosh_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(cosh_rf_r); return(NULL);}


/* -------- sinh -------- */
static s7_pointer s7_dl_sinh(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_sinh_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_sinh_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "sinh", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)sinh(s7_dl_sinh_0)));
}
static s7_double sinh_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(sinh(x));}
static s7_rf_t sinh_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(sinh_rf_r); return(NULL);}


/* -------- tanh -------- */
static s7_pointer s7_dl_tanh(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_tanh_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_tanh_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "tanh", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)tanh(s7_dl_tanh_0)));
}
static s7_double tanh_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(tanh(x));}
static s7_rf_t tanh_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(tanh_rf_r); return(NULL);}


/* -------- acos -------- */
static s7_pointer s7_dl_acos(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_acos_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_acos_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "acos", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)acos(s7_dl_acos_0)));
}
static s7_double acos_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(acos(x));}
static s7_rf_t acos_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(acos_rf_r); return(NULL);}


/* -------- asin -------- */
static s7_pointer s7_dl_asin(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_asin_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_asin_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "asin", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)asin(s7_dl_asin_0)));
}
static s7_double asin_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(asin(x));}
static s7_rf_t asin_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(asin_rf_r); return(NULL);}


/* -------- atan -------- */
static s7_pointer s7_dl_atan(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_atan_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_atan_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "atan", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)atan(s7_dl_atan_0)));
}
static s7_double atan_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(atan(x));}
static s7_rf_t atan_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(atan_rf_r); return(NULL);}


/* -------- atan2 -------- */
static s7_pointer s7_dl_atan2(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_atan2_0;
  double s7_dl_atan2_1;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_atan2_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "atan2", 1, s7_car(arg), "real"));
  arg = s7_cdr(arg);
  if (s7_is_real(s7_car(arg)))
    s7_dl_atan2_1 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "atan2", 2, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)atan2(s7_dl_atan2_0, s7_dl_atan2_1)));
}
static s7_double atan2_rf_r(s7_scheme *sc, s7_pointer **p)
  {s7_rf_t f; s7_double x, y; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); f = (s7_rf_t)(**p); (*p)++; y = f(sc, p); return(atan2(x, y));}
static s7_rf_t atan2_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(atan2_rf_r); return(NULL);}


/* -------- acosh -------- */
static s7_pointer s7_dl_acosh(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_acosh_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_acosh_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "acosh", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)acosh(s7_dl_acosh_0)));
}
static s7_double acosh_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(acosh(x));}
static s7_rf_t acosh_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(acosh_rf_r); return(NULL);}


/* -------- asinh -------- */
static s7_pointer s7_dl_asinh(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_asinh_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_asinh_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "asinh", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)asinh(s7_dl_asinh_0)));
}
static s7_double asinh_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(asinh(x));}
static s7_rf_t asinh_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(asinh_rf_r); return(NULL);}


/* -------- atanh -------- */
static s7_pointer s7_dl_atanh(s7_scheme *sc, s7_pointer args)
{
  s7_pointer arg;
  double s7_dl_atanh_0;
  arg = args;
  if (s7_is_real(s7_car(arg)))
    s7_dl_atanh_0 = (double)s7_number_to_real(sc, s7_car(arg));
  else return(s7_wrong_type_arg_error(sc, "atanh", 0, s7_car(arg), "real"));
  return(s7_make_real(sc, (s7_double)atanh(s7_dl_atanh_0)));
}
static s7_double atanh_rf_r(s7_scheme *sc, s7_pointer **p){s7_rf_t f; s7_double x; f = (s7_rf_t)(**p); (*p)++; x = f(sc, p); return(atanh(x));}
static s7_rf_t atanh_rf(s7_scheme *sc, s7_pointer expr) {if (s7_arg_to_rf(sc, s7_cadr(expr))) return(atanh_rf_r); return(NULL);}


static s7_pointer g_remquo(s7_scheme *sc, s7_pointer args)
{
  if (s7_is_real(s7_car(args)))
    {
      if (s7_is_real(s7_cadr(args)))
        {
          int quo = 0;
          double rem;
          rem = remquo(s7_number_to_real(sc, s7_car(args)), s7_number_to_real(sc, s7_cadr(args)), &quo);
          return(s7_list(sc, 2, s7_make_real(sc, rem), s7_make_integer(sc, quo)));
        }
      return(s7_wrong_type_arg_error(sc, "remquo", 2, s7_cadr(args), "a real"));
     }
  return(s7_wrong_type_arg_error(sc, "remquo", 1, s7_car(args), "a real"));
}
static s7_pointer g_frexp(s7_scheme *sc, s7_pointer args)
{
  if (s7_is_real(s7_car(args)))
    {
      int ex = 0;
      double frac;
      frac = frexp(s7_number_to_real(sc, s7_car(args)), &ex);
      return(s7_list(sc, 2, s7_make_real(sc, frac), s7_make_integer(sc, ex)));
     }
  return(s7_wrong_type_arg_error(sc, "frexp", 1, s7_car(args), "a real"));
}
static s7_pointer g_modf(s7_scheme *sc, s7_pointer args)
{
  if (s7_is_real(s7_car(args)))
    {
      double frac, ip = 0.0;
      frac = modf(s7_number_to_real(sc, s7_car(args)), &ip);
      return(s7_list(sc, 2, s7_make_real(sc, frac), s7_make_real(sc, ip)));
     }
  return(s7_wrong_type_arg_error(sc, "modf", 1, s7_car(args), "a real"));
}

void libm_s7_init(s7_scheme *sc);
void libm_s7_init(s7_scheme *sc)
{
  s7_pointer cur_env;
  s7_pointer pl_ir, pl_ds, pl_dri, pl_dr;
  {
    s7_pointer s, d, r, i;
    s = s7_make_symbol(sc, "string?");
    d = s7_make_symbol(sc, "float?");
    r = s7_make_symbol(sc, "real?");
    i = s7_make_symbol(sc, "integer?");

    pl_ir = s7_make_signature(sc, 2, i, r);
    pl_ds = s7_make_signature(sc, 2, d, s);
    pl_dri = s7_make_signature(sc, 3, d, r, i);
    pl_dr = s7_make_signature(sc, 2, d, r);
  }

  cur_env = s7_outlet(sc, s7_curlet(sc));

  s7_define(sc, cur_env, s7_make_symbol(sc, "M_SQRT1_2"), s7_make_real(sc, (s7_double)M_SQRT1_2));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_SQRT2"), s7_make_real(sc, (s7_double)M_SQRT2));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_2_SQRTPI"), s7_make_real(sc, (s7_double)M_2_SQRTPI));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_2_PI"), s7_make_real(sc, (s7_double)M_2_PI));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_1_PI"), s7_make_real(sc, (s7_double)M_1_PI));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_PI_4"), s7_make_real(sc, (s7_double)M_PI_4));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_PI_2"), s7_make_real(sc, (s7_double)M_PI_2));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_PI"), s7_make_real(sc, (s7_double)M_PI));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_LN10"), s7_make_real(sc, (s7_double)M_LN10));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_LN2"), s7_make_real(sc, (s7_double)M_LN2));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_LOG10E"), s7_make_real(sc, (s7_double)M_LOG10E));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_LOG2E"), s7_make_real(sc, (s7_double)M_LOG2E));
  s7_define(sc, cur_env, s7_make_symbol(sc, "M_E"), s7_make_real(sc, (s7_double)M_E));
  s7_define(sc, cur_env, s7_make_symbol(sc, "FP_NORMAL"), s7_make_integer(sc, (s7_int)FP_NORMAL));
  s7_define(sc, cur_env, s7_make_symbol(sc, "FP_SUBNORMAL"), s7_make_integer(sc, (s7_int)FP_SUBNORMAL));
  s7_define(sc, cur_env, s7_make_symbol(sc, "FP_ZERO"), s7_make_integer(sc, (s7_int)FP_ZERO));
  s7_define(sc, cur_env, s7_make_symbol(sc, "FP_INFINITE"), s7_make_integer(sc, (s7_int)FP_INFINITE));
  s7_define(sc, cur_env, s7_make_symbol(sc, "FP_NAN"), s7_make_integer(sc, (s7_int)FP_NAN));

#ifdef __LDBL_DENORM_MIN__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LDBL_DENORM_MIN__"), s7_make_real(sc, (s7_double)__LDBL_DENORM_MIN__));
#endif
#ifdef __FLT_MAX__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_MAX__"), s7_make_real(sc, (s7_double)__FLT_MAX__));
#endif
#ifdef __FLT_DENORM_MIN__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_DENORM_MIN__"), s7_make_real(sc, (s7_double)__FLT_DENORM_MIN__));
#endif
#ifdef __DBL_EPSILON__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DBL_EPSILON__"), s7_make_real(sc, (s7_double)__DBL_EPSILON__));
#endif
#ifdef __LDBL_EPSILON__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LDBL_EPSILON__"), s7_make_real(sc, (s7_double)__LDBL_EPSILON__));
#endif
#ifdef __DBL_MIN__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DBL_MIN__"), s7_make_real(sc, (s7_double)__DBL_MIN__));
#endif
#ifdef __DBL_MAX__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DBL_MAX__"), s7_make_real(sc, (s7_double)__DBL_MAX__));
#endif
#ifdef __LDBL_MIN__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LDBL_MIN__"), s7_make_real(sc, (s7_double)__LDBL_MIN__));
#endif
#ifdef __FLT_EPSILON__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_EPSILON__"), s7_make_real(sc, (s7_double)__FLT_EPSILON__));
#endif
#ifdef __LDBL_MAX__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LDBL_MAX__"), s7_make_real(sc, (s7_double)__LDBL_MAX__));
#endif
#ifdef __DBL_DENORM_MIN__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DBL_DENORM_MIN__"), s7_make_real(sc, (s7_double)__DBL_DENORM_MIN__));
#endif
#ifdef __FLT_MIN__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_MIN__"), s7_make_real(sc, (s7_double)__FLT_MIN__));
#endif
#ifdef __LDBL_DIG__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LDBL_DIG__"), s7_make_integer(sc, (s7_int)__LDBL_DIG__));
#endif
#ifdef __LDBL_MIN_10_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LDBL_MIN_10_EXP__"), s7_make_integer(sc, (s7_int)__LDBL_MIN_10_EXP__));
#endif
#ifdef __DBL_MAX_10_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DBL_MAX_10_EXP__"), s7_make_integer(sc, (s7_int)__DBL_MAX_10_EXP__));
#endif
#ifdef __FLT_MIN_10_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_MIN_10_EXP__"), s7_make_integer(sc, (s7_int)__FLT_MIN_10_EXP__));
#endif
#ifdef __INTMAX_MAX__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__INTMAX_MAX__"), s7_make_integer(sc, (s7_int)__INTMAX_MAX__));
#endif
#ifdef __LDBL_MAX_10_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LDBL_MAX_10_EXP__"), s7_make_integer(sc, (s7_int)__LDBL_MAX_10_EXP__));
#endif
#ifdef __LDBL_MIN_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LDBL_MIN_EXP__"), s7_make_integer(sc, (s7_int)__LDBL_MIN_EXP__));
#endif
#ifdef __DBL_MANT_DIG__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DBL_MANT_DIG__"), s7_make_integer(sc, (s7_int)__DBL_MANT_DIG__));
#endif
#ifdef __FLT_MAX_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_MAX_EXP__"), s7_make_integer(sc, (s7_int)__FLT_MAX_EXP__));
#endif
#ifdef __INT_MAX__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__INT_MAX__"), s7_make_integer(sc, (s7_int)__INT_MAX__));
#endif
#ifdef __FLT_DIG__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_DIG__"), s7_make_integer(sc, (s7_int)__FLT_DIG__));
#endif
#ifdef __LDBL_MANT_DIG__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LDBL_MANT_DIG__"), s7_make_integer(sc, (s7_int)__LDBL_MANT_DIG__));
#endif
#ifdef __LONG_MAX__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LONG_MAX__"), s7_make_integer(sc, (s7_int)__LONG_MAX__));
#endif
#ifdef __FLT_MAX_10_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_MAX_10_EXP__"), s7_make_integer(sc, (s7_int)__FLT_MAX_10_EXP__));
#endif
#ifdef __FLT_RADIX__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_RADIX__"), s7_make_integer(sc, (s7_int)__FLT_RADIX__));
#endif
#ifdef __FLT_MANT_DIG__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_MANT_DIG__"), s7_make_integer(sc, (s7_int)__FLT_MANT_DIG__));
#endif
#ifdef __FLT_MIN_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__FLT_MIN_EXP__"), s7_make_integer(sc, (s7_int)__FLT_MIN_EXP__));
#endif
#ifdef __LONG_LONG_MAX__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LONG_LONG_MAX__"), s7_make_integer(sc, (s7_int)__LONG_LONG_MAX__));
#endif
#ifdef __DBL_MAX_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DBL_MAX_EXP__"), s7_make_integer(sc, (s7_int)__DBL_MAX_EXP__));
#endif
#ifdef __BIGGEST_ALIGNMENT__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__BIGGEST_ALIGNMENT__"), s7_make_integer(sc, (s7_int)__BIGGEST_ALIGNMENT__));
#endif
#ifdef __DECIMAL_DIG__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DECIMAL_DIG__"), s7_make_integer(sc, (s7_int)__DECIMAL_DIG__));
#endif
#ifdef __DBL_DIG__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DBL_DIG__"), s7_make_integer(sc, (s7_int)__DBL_DIG__));
#endif
#ifdef __LDBL_MAX_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__LDBL_MAX_EXP__"), s7_make_integer(sc, (s7_int)__LDBL_MAX_EXP__));
#endif
#ifdef __DBL_MIN_10_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DBL_MIN_10_EXP__"), s7_make_integer(sc, (s7_int)__DBL_MIN_10_EXP__));
#endif
#ifdef __DBL_MIN_EXP__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__DBL_MIN_EXP__"), s7_make_integer(sc, (s7_int)__DBL_MIN_EXP__));
#endif
#ifdef __CHAR_BIT__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__CHAR_BIT__"), s7_make_integer(sc, (s7_int)__CHAR_BIT__));
#endif
#ifdef __SIZEOF_DOUBLE__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__SIZEOF_DOUBLE__"), s7_make_integer(sc, (s7_int)__SIZEOF_DOUBLE__));
#endif
#ifdef __SIZEOF_SHORT__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__SIZEOF_SHORT__"), s7_make_integer(sc, (s7_int)__SIZEOF_SHORT__));
#endif
#ifdef __SIZEOF_FLOAT__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__SIZEOF_FLOAT__"), s7_make_integer(sc, (s7_int)__SIZEOF_FLOAT__));
#endif
#ifdef __SIZEOF_SIZE_T__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__SIZEOF_SIZE_T__"), s7_make_integer(sc, (s7_int)__SIZEOF_SIZE_T__));
#endif
#ifdef __SIZEOF_LONG_DOUBLE__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__SIZEOF_LONG_DOUBLE__"), s7_make_integer(sc, (s7_int)__SIZEOF_LONG_DOUBLE__));
#endif
#ifdef __SIZEOF_LONG__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__SIZEOF_LONG__"), s7_make_integer(sc, (s7_int)__SIZEOF_LONG__));
#endif
#ifdef __SIZEOF_POINTER__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__SIZEOF_POINTER__"), s7_make_integer(sc, (s7_int)__SIZEOF_POINTER__));
#endif
#ifdef __SIZEOF_INT__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__SIZEOF_INT__"), s7_make_integer(sc, (s7_int)__SIZEOF_INT__));
#endif
#ifdef __SIZEOF_LONG_LONG__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__SIZEOF_LONG_LONG__"), s7_make_integer(sc, (s7_int)__SIZEOF_LONG_LONG__));
#endif
#ifdef __VERSION__
  s7_define(sc, cur_env, s7_make_symbol(sc, "__VERSION__"), s7_make_string(sc, (char*)__VERSION__));
#endif

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "modf"),
            s7_make_typed_function(sc, "modf", g_modf, 1, 0, false, "(modf x) returns a list: (int-part frac-part) -- this is not the same as fmod!", NULL));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "frexp"),
            s7_make_typed_function(sc, "frexp", g_frexp, 1, 0, false, "(frexp x) returns a list: (fraction exponent)", NULL));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "remquo"),
            s7_make_typed_function(sc, "remquo", g_remquo, 2, 0, false, "(remquo x y) returns a list: (remainder messed-up-quotient)", NULL));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "atanh"),
            s7_make_typed_function(sc, "atanh", s7_dl_atanh, 1, 0, false, "double atanh(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "asinh"),
            s7_make_typed_function(sc, "asinh", s7_dl_asinh, 1, 0, false, "double asinh(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "acosh"),
            s7_make_typed_function(sc, "acosh", s7_dl_acosh, 1, 0, false, "double acosh(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "atan2"),
            s7_make_typed_function(sc, "atan2", s7_dl_atan2, 2, 0, false, "double atan2(double double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "atan"),
            s7_make_typed_function(sc, "atan", s7_dl_atan, 1, 0, false, "double atan(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "asin"),
            s7_make_typed_function(sc, "asin", s7_dl_asin, 1, 0, false, "double asin(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "acos"),
            s7_make_typed_function(sc, "acos", s7_dl_acos, 1, 0, false, "double acos(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "tanh"),
            s7_make_typed_function(sc, "tanh", s7_dl_tanh, 1, 0, false, "double tanh(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "sinh"),
            s7_make_typed_function(sc, "sinh", s7_dl_sinh, 1, 0, false, "double sinh(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "cosh"),
            s7_make_typed_function(sc, "cosh", s7_dl_cosh, 1, 0, false, "double cosh(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "tan"),
            s7_make_typed_function(sc, "tan", s7_dl_tan, 1, 0, false, "double tan(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "sin"),
            s7_make_typed_function(sc, "sin", s7_dl_sin, 1, 0, false, "double sin(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "cos"),
            s7_make_typed_function(sc, "cos", s7_dl_cos, 1, 0, false, "double cos(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "sqrt"),
            s7_make_typed_function(sc, "sqrt", s7_dl_sqrt, 1, 0, false, "double sqrt(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "log"),
            s7_make_typed_function(sc, "log", s7_dl_log, 1, 0, false, "double log(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "exp"),
            s7_make_typed_function(sc, "exp", s7_dl_exp, 1, 0, false, "double exp(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "remainder"),
            s7_make_typed_function(sc, "remainder", s7_dl_remainder, 2, 0, false, "double remainder(double double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "round"),
            s7_make_typed_function(sc, "round", s7_dl_round, 1, 0, false, "double round(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "floor"),
            s7_make_typed_function(sc, "floor", s7_dl_floor, 1, 0, false, "double floor(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "signbit"),
            s7_make_typed_function(sc, "signbit", s7_dl_signbit, 1, 0, false, "int signbit(double)", pl_ir));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "isnormal"),
            s7_make_typed_function(sc, "isnormal", s7_dl_isnormal, 1, 0, false, "int isnormal(double)", pl_ir));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "isnan"),
            s7_make_typed_function(sc, "isnan", s7_dl_isnan, 1, 0, false, "int isnan(double)", pl_ir));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "isinf"),
            s7_make_typed_function(sc, "isinf", s7_dl_isinf, 1, 0, false, "int isinf(double)", pl_ir));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "isfinite"),
            s7_make_typed_function(sc, "isfinite", s7_dl_isfinite, 1, 0, false, "int isfinite(double)", pl_ir));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "fpclassify"),
            s7_make_typed_function(sc, "fpclassify", s7_dl_fpclassify, 1, 0, false, "int fpclassify(double)", pl_ir));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "nexttoward"),
            s7_make_typed_function(sc, "nexttoward", s7_dl_nexttoward, 2, 0, false, "double nexttoward(double double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "nextafter"),
            s7_make_typed_function(sc, "nextafter", s7_dl_nextafter, 2, 0, false, "double nextafter(double double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "nan"),
            s7_make_typed_function(sc, "nan", s7_dl_nan, 1, 0, false, "double nan(char*)", pl_ds));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "copysign"),
            s7_make_typed_function(sc, "copysign", s7_dl_copysign, 2, 0, false, "double copysign(double double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "tgamma"),
            s7_make_typed_function(sc, "tgamma", s7_dl_tgamma, 1, 0, false, "double tgamma(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "fdim"),
            s7_make_typed_function(sc, "fdim", s7_dl_fdim, 2, 0, false, "double fdim(double double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "pow"),
            s7_make_typed_function(sc, "pow", s7_dl_pow, 2, 0, false, "double pow(double double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "hypot"),
            s7_make_typed_function(sc, "hypot", s7_dl_hypot, 2, 0, false, "double hypot(double double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "cbrt"),
            s7_make_typed_function(sc, "cbrt", s7_dl_cbrt, 1, 0, false, "double cbrt(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "ilogb"),
            s7_make_typed_function(sc, "ilogb", s7_dl_ilogb, 1, 0, false, "int ilogb(double)", pl_ir));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "log2"),
            s7_make_typed_function(sc, "log2", s7_dl_log2, 1, 0, false, "double log2(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "log1p"),
            s7_make_typed_function(sc, "log1p", s7_dl_log1p, 1, 0, false, "double log1p(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "log10"),
            s7_make_typed_function(sc, "log10", s7_dl_log10, 1, 0, false, "double log10(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "expm1"),
            s7_make_typed_function(sc, "expm1", s7_dl_expm1, 1, 0, false, "double expm1(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "exp2"),
            s7_make_typed_function(sc, "exp2", s7_dl_exp2, 1, 0, false, "double exp2(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "scalbn"),
            s7_make_typed_function(sc, "scalbn", s7_dl_scalbn, 2, 0, false, "double scalbn(double int)", pl_dri));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "ldexp"),
            s7_make_typed_function(sc, "ldexp", s7_dl_ldexp, 2, 0, false, "double ldexp(double int)", pl_dri));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "fmod"),
            s7_make_typed_function(sc, "fmod", s7_dl_fmod, 2, 0, false, "double fmod(double double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "trunc"),
            s7_make_typed_function(sc, "trunc", s7_dl_trunc, 1, 0, false, "double trunc(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "llround"),
            s7_make_typed_function(sc, "llround", s7_dl_llround, 1, 0, false, "int llround(double)", pl_ir));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "llrint"),
            s7_make_typed_function(sc, "llrint", s7_dl_llrint, 1, 0, false, "int llrint(double)", pl_ir));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "rint"),
            s7_make_typed_function(sc, "rint", s7_dl_rint, 1, 0, false, "double rint(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "fma"),
            s7_make_typed_function(sc, "fma", s7_dl_fma, 3, 0, false, "double fma(double double double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "scalbln"),
            s7_make_typed_function(sc, "scalbln", s7_dl_scalbln, 2, 0, false, "double scalbln(double int)", pl_dri));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "nearbyint"),
            s7_make_typed_function(sc, "nearbyint", s7_dl_nearbyint, 1, 0, false, "double nearbyint(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "ceil"),
            s7_make_typed_function(sc, "ceil", s7_dl_ceil, 1, 0, false, "double ceil(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "fabs"),
            s7_make_typed_function(sc, "fabs", s7_dl_fabs, 1, 0, false, "double fabs(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "lgamma"),
            s7_make_typed_function(sc, "lgamma", s7_dl_lgamma, 1, 0, false, "double lgamma(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "erfc"),
            s7_make_typed_function(sc, "erfc", s7_dl_erfc, 1, 0, false, "double erfc(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "erf"),
            s7_make_typed_function(sc, "erf", s7_dl_erf, 1, 0, false, "double erf(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "j1"),
            s7_make_typed_function(sc, "j1", s7_dl_j1, 1, 0, false, "double j1(double)", pl_dr));

  s7_define(sc, cur_env,
            s7_make_symbol(sc, "j0"),
            s7_make_typed_function(sc, "j0", s7_dl_j0, 1, 0, false, "Bessel j0", pl_dr));

  /* rf optimizer connections */
  s7_rf_set_function(s7_name_to_value(sc, "atanh"), atanh_rf);
  s7_rf_set_function(s7_name_to_value(sc, "asinh"), asinh_rf);
  s7_rf_set_function(s7_name_to_value(sc, "acosh"), acosh_rf);
  s7_rf_set_function(s7_name_to_value(sc, "atan2"), atan2_rf);
  s7_rf_set_function(s7_name_to_value(sc, "atan"), atan_rf);
  s7_rf_set_function(s7_name_to_value(sc, "asin"), asin_rf);
  s7_rf_set_function(s7_name_to_value(sc, "acos"), acos_rf);
  s7_rf_set_function(s7_name_to_value(sc, "tanh"), tanh_rf);
  s7_rf_set_function(s7_name_to_value(sc, "sinh"), sinh_rf);
  s7_rf_set_function(s7_name_to_value(sc, "cosh"), cosh_rf);
  s7_rf_set_function(s7_name_to_value(sc, "tan"), tan_rf);
  s7_rf_set_function(s7_name_to_value(sc, "sin"), sin_rf);
  s7_rf_set_function(s7_name_to_value(sc, "cos"), cos_rf);
  s7_rf_set_function(s7_name_to_value(sc, "sqrt"), sqrt_rf);
  s7_rf_set_function(s7_name_to_value(sc, "log"), log_rf);
  s7_rf_set_function(s7_name_to_value(sc, "exp"), exp_rf);
  s7_rf_set_function(s7_name_to_value(sc, "remainder"), remainder_rf);
  s7_rf_set_function(s7_name_to_value(sc, "round"), round_rf);
  s7_rf_set_function(s7_name_to_value(sc, "floor"), floor_rf);
  s7_rf_set_function(s7_name_to_value(sc, "nexttoward"), nexttoward_rf);
  s7_rf_set_function(s7_name_to_value(sc, "nextafter"), nextafter_rf);
  s7_rf_set_function(s7_name_to_value(sc, "copysign"), copysign_rf);
  s7_rf_set_function(s7_name_to_value(sc, "tgamma"), tgamma_rf);
  s7_rf_set_function(s7_name_to_value(sc, "fdim"), fdim_rf);
  s7_rf_set_function(s7_name_to_value(sc, "pow"), pow_rf);
  s7_rf_set_function(s7_name_to_value(sc, "hypot"), hypot_rf);
  s7_rf_set_function(s7_name_to_value(sc, "cbrt"), cbrt_rf);
  s7_rf_set_function(s7_name_to_value(sc, "log2"), log2_rf);
  s7_rf_set_function(s7_name_to_value(sc, "log1p"), log1p_rf);
  s7_rf_set_function(s7_name_to_value(sc, "log10"), log10_rf);
  s7_rf_set_function(s7_name_to_value(sc, "expm1"), expm1_rf);
  s7_rf_set_function(s7_name_to_value(sc, "exp2"), exp2_rf);
  s7_rf_set_function(s7_name_to_value(sc, "fmod"), fmod_rf);
  s7_rf_set_function(s7_name_to_value(sc, "trunc"), trunc_rf);
  s7_rf_set_function(s7_name_to_value(sc, "rint"), rint_rf);
  s7_rf_set_function(s7_name_to_value(sc, "nearbyint"), nearbyint_rf);
  s7_rf_set_function(s7_name_to_value(sc, "ceil"), ceil_rf);
  s7_rf_set_function(s7_name_to_value(sc, "fabs"), fabs_rf);
  s7_rf_set_function(s7_name_to_value(sc, "lgamma"), lgamma_rf);
  s7_rf_set_function(s7_name_to_value(sc, "erfc"), erfc_rf);
  s7_rf_set_function(s7_name_to_value(sc, "erf"), erf_rf);
  s7_rf_set_function(s7_name_to_value(sc, "j1"), j1_rf);
  s7_rf_set_function(s7_name_to_value(sc, "j0"), j0_rf);

  /* if optimizer connections */
  s7_if_set_function(s7_name_to_value(sc, "signbit"), signbit_if);
  s7_if_set_function(s7_name_to_value(sc, "isnormal"), isnormal_if);
  s7_if_set_function(s7_name_to_value(sc, "isnan"), isnan_if);
  s7_if_set_function(s7_name_to_value(sc, "isinf"), isinf_if);
  s7_if_set_function(s7_name_to_value(sc, "isfinite"), isfinite_if);
  s7_if_set_function(s7_name_to_value(sc, "fpclassify"), fpclassify_if);
  s7_if_set_function(s7_name_to_value(sc, "ilogb"), ilogb_if);
  s7_if_set_function(s7_name_to_value(sc, "llround"), llround_if);
  s7_if_set_function(s7_name_to_value(sc, "llrint"), llrint_if);
}
