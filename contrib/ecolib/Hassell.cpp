#include <admodel.h>
#include <df1b2fun.h>
#include <adrndeff.h> 

/*---------------------------------------------------------3rd variable is a scalar*/
/*---------------------------------------------2nd variable is a scalar*/
/**  Hassell function scalar
\param x independent variable; data scalar
\param a ; differentiable scalar
\param b ; differentiable scalar
\param c ; differentiable scalar
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
dvariable Hassell(const double& x,  const prevariable& a,  const prevariable& b,  const prevariable& c)
{
    RETURN_ARRAYS_INCREMENT();	
	dvariable y;
	y=a*x/(pow(b+x,c));

	RETURN_ARRAYS_DECREMENT();
	return (y);
}
/**  Hassell function vectorized
\param x independent variable; data vector
\param a ; differentiable scalar
\param b ; differentiable scalar
\param c ; differentiable scalar
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
dvar_vector Hassell(const dvector& x,  const prevariable& a,  const prevariable& b,  const prevariable& c)
{
    RETURN_ARRAYS_INCREMENT();	
	dvar_vector y;
	y=a*elem_div(x, (pow(b+x,c)));

	RETURN_ARRAYS_DECREMENT();	
    return (y);
}
/**  Hassell function vectorized
\param x independent variable; data vector
\param a ; differentiable vector
\param b ; differentiable scalar
\param c ; differentiable scalar
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
dvar_vector Hassell(const dvector& x,  const dvar_vector& a,  const prevariable& b,  const prevariable& c)
{
    RETURN_ARRAYS_INCREMENT();	
	dvar_vector y;
	y=elem_prod(a, elem_div(x, (pow(b+x,c))));

	RETURN_ARRAYS_DECREMENT();	
    return (y);
}
/*---------------------------------------------2nd variable is a vector*/
/**  Hassell function vectorized
\param x independent variable; data vector
\param a ; differentiable scalar
\param b ; differentiable vector
\param c ; differentiable scalar
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
dvar_vector Hassell(const dvector& x,  const prevariable& a,  const dvar_vector& b,  const prevariable& c)
{
    RETURN_ARRAYS_INCREMENT();	
	dvar_vector y;
	y=a*elem_div(x, (pow(b+x,c)));

	RETURN_ARRAYS_DECREMENT();	
    return (y);
}
/**  Hassell function vectorized
\param x independent variable; data vector
\param a ; differentiable vector
\param b ; differentiable vector
\param c ; differentiable scalar
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
dvar_vector Hassell(const dvector& x,  const dvar_vector& a,  const dvar_vector& b,  const prevariable& c)
{
    RETURN_ARRAYS_INCREMENT();	
	dvar_vector y;
	y=elem_prod(a, elem_div(x, (pow(b+x,c))));

	RETURN_ARRAYS_DECREMENT();	
    return (y);
}
/*---------------------------------------------------------3rd variable is a vector*/
/*---------------------------------------------2nd variable is a scalar*/
/**  Hassell function vectorized
\param x independent variable; data vector
\param a ; differentiable scalar
\param b ; differentiable scalar
\param c ; differentiable vector
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
dvar_vector Hassell(const dvector& x,  const prevariable& a,  const prevariable& b,  const dvar_vector& c)
{
    RETURN_ARRAYS_INCREMENT();	
	dvar_vector y;
	y=a*elem_div(x, (pow(b+x,c)));

	RETURN_ARRAYS_DECREMENT();	
    return (y);
}
/**  Hassell function vectorized
\param x independent variable; data vector
\param a ; differentiable vector
\param b ; differentiable scalar
\param c ; differentiable vector
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
dvar_vector Hassell(const dvector& x,  const dvar_vector& a,  const prevariable& b,  const dvar_vector& c)
{
    RETURN_ARRAYS_INCREMENT();	
	dvar_vector y;
	y=elem_prod(a, elem_div(x, (pow(b+x,c))));

	RETURN_ARRAYS_DECREMENT();	
    return (y);
}
/*---------------------------------------------2nd variable is a vector*/
/**  Hassell function vectorized
\param x independent variable; data vector
\param a ; differentiable scalar
\param b ; differentiable vector
\param c ; differentiable vector
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
dvar_vector Hassell(const dvector& x,  const prevariable& a,  const dvar_vector& b,  const dvar_vector& c)
{
    RETURN_ARRAYS_INCREMENT();	
	dvar_vector y;
	y=a*elem_div(x, (pow(b+x,c)));

	RETURN_ARRAYS_DECREMENT();	
    return (y);
}
/**  Hassell function vectorized
\param x independent variable; data vector
\param a ; differentiable vector
\param b ; differentiable vector
\param c ; differentiable vector
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
dvar_vector Hassell(const dvector& x,  const dvar_vector& a,  const dvar_vector& b,  const dvar_vector& c)
{
    RETURN_ARRAYS_INCREMENT();	
	dvar_vector y;
	y=elem_prod(a, elem_div(x, (pow(b+x,c))));

	RETURN_ARRAYS_DECREMENT();	
    return (y);
}
/*---------------------------------------------------------3rd variable is a scalar in a random effects model*/
/*---------------------------------------------2nd variable is a scalar in a random effects model*/
/*----------------------1st varible contains random effects*/
/** Hassell function random effects scalar
\param x independent variable; data scalar
\param a ; differentiable scalar in a random effects model
\param b ; differentiable scalar in a random effects model
\param c ; differentiable scalar in a random effects model
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
df1b2variable Hassell(const double& x,  const df1b2variable& a,  const df1b2variable& b,  const df1b2variable& c)
{
	df1b2variable y;
	y=a*x/(pow(b+x,c));

    return (y);
}
/** Hassell function random effects vectorized
\param x independent variable; data vector
\param a ; differentiable scalar in a random effects model
\param b ; differentiable scalar in a random effects model
\param c ; differentiable scalar in a random effects model
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
df1b2vector Hassell(const dvector& x,  const df1b2variable& a,  const df1b2variable& b,  const df1b2variable& c)
{
	df1b2vector y;
	y=a*elem_div(x, (pow(b+x,c)));

    return(y);
}
/** Hassell function random effects vectorized
\param x independent variable; data vector
\param a ; differentiable vector in a random effects model
\param b ; differentiable scalar in a random effects model
\param c ; differentiable scalar in a random effects model
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
df1b2vector Hassell(const dvector& x,  const df1b2vector& a,  const df1b2variable& b,  const df1b2variable& c)
{
	df1b2vector y;
	y=elem_prod(a, elem_div(x, (pow(b+x,c))));

    return(y);
}

/*---------------------------------------------2nd variable is a vector in a random effects model*/
/*----------------------1st varible contains random effects*/
/** Hassell function random effects vectorized
\param x independent variable; data vector
\param a ; differentiable scalar in a random effects model
\param b ; differentiable vector in a random effects model
\param c ; differentiable scalar in a random effects model
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
df1b2vector Hassell(const dvector& x,  const df1b2variable& a,  const df1b2vector& b,  const df1b2variable& c)
{
	df1b2vector y;
	y=a*elem_div(x, (pow(b+x,c)));

    return(y);
}
/** Hassell function random effects vectorized
\param x independent variable; data vector
\param a ; differentiable vector in a random effects model
\param b ; differentiable vector in a random effects model
\param c ; differentiable scalar in a random effects model
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
df1b2vector Hassell(const dvector& x,  const df1b2vector& a,  const df1b2vector& b,  const df1b2variable& c)
{
	df1b2vector y;
	y=elem_prod(a, elem_div(x, (pow(b+x,c))));

    return(y);
}


/*---------------------------------------------------------3rd variable is a vector in a random effects model*/
/*---------------------------------------------2nd variable is a scalar in a random effects model*/
/*----------------------1st varible contains random effects*/
/** Hassell function random effects vectorized
\param x independent variable; data vector
\param a ; differentiable scalar in a random effects model
\param b ; differentiable scalar in a random effects model
\param c ; differentiable vector in a random effects model
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
df1b2vector Hassell(const dvector& x,  const df1b2variable& a,  const df1b2variable& b,  const df1b2vector& c)
{
	df1b2vector y;
	y=a*elem_div(x, (pow(b+x,c)));

    return(y);
}
/** Hassell function random effects vectorized
\param x independent variable; data vector
\param a ; differentiable vector in a random effects model
\param b ; differentiable scalar in a random effects model
\param c ; differentiable vector in a random effects model
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
df1b2vector Hassell(const dvector& x,  const df1b2vector& a,  const df1b2variable& b,  const df1b2vector& c)
{
	df1b2vector y;
	y=elem_prod(a, elem_div(x, (pow(b+x,c))));

    return(y);
}

/*---------------------------------------------2nd variable is a vector in a random effects model*/
/*----------------------1st varible contains random effects*/
/** Hassell function random effects vectorized
\param x independent variable; data vector
\param a ; differentiable scalar in a random effects model
\param b ; differentiable vector in a random effects model
\param c ; differentiable vector in a random effects model
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
df1b2vector Hassell(const dvector& x,  const df1b2variable& a,  const df1b2vector& b,  const df1b2vector& c)
{
	df1b2vector y;
	y=a*elem_div(x, (pow(b+x,c)));

    return(y);
}
/** Hassell function random effects vectorized
\param x independent variable; data vector
\param a ; differentiable vector in a random effects model
\param b ; differentiable vector in a random effects model
\param c ; differentiable vector in a random effects model
\return  \f$ \frac{ax}{(b+x)^c} \f$
\ingroup ECOL
**/
df1b2vector Hassell(const dvector& x,  const df1b2vector& a,  const df1b2vector& b,  const df1b2vector& c)
{
	df1b2vector y;
	y=elem_prod(a, elem_div(x, (pow(b+x,c))));

    return(y);
}
