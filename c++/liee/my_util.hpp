/*!
 * my_util.hpp
 *
 *  Created on: 6-Jan-2012
 *      Author: quark
 *
 * Contains a rather unstructured collection of utility routines, which are
 * both simple and generic to help reuse and avoid duplication.
 */


#ifndef MY_UTIL_H_
#define MY_UTIL_H_


#define AT(a) at(a);
//#define AT(a) [a];

#ifdef LOG_ENABLED
	#define DEBUG_SHOW(a) LOG4CXX_DEBUG(logger, #a << "=" << (a) );
	#define DEBUG_SHOW2(a,b) LOG4CXX_DEBUG(logger, #a << "=" << (a) << "\t" << #b << "=" << (b) );
	#define DEBUG_SHOW3(a,b,c) LOG4CXX_DEBUG(logger, #a << "=" << (a) << "\t" << #b << "=" << (b) << "\t" << #c << "=" << (c) );
	#define DEBUG_SHOW4(a,b,c,d) LOG4CXX_DEBUG(logger, #a << "=" << (a) << "\t" << #b << "=" << (b) << "\t" << #c << "=" << (c) << "\t" << #d << "=" << (d) );
	#define INFO_SHOW(a) LOG4CXX_INFO(logger, #a << "=" << (a) );
	#define INFO_SHOW2(a,b) LOG4CXX_INFO(logger, #a << "=" << (a) << "\t" << #b << "=" << (b) );
	#define INFO_SHOW3(a,b,c) LOG4CXX_INFO(logger, #a << "=" << (a) << "\t" << #b << "=" << (b) << "\t" << #c << "=" << (c) );
	#define INFO_SHOW4(a,b,c,d) LOG4CXX_INFO(logger, #a << "=" << (a) << "\t" << #b << "=" << (b) << "\t" << #c << "=" << (c) << "\t" << #d << "=" << (d) );
	#define DBG_TRACE LOG4CXX_DEBUG(logger, __FILE__ << " :: " << __FUNCTION__ << "() #" << __LINE__);

	#define DECLARE_LOGGER log4cxx::LoggerPtr logger;
	#define GET_LOGGER(name) logger = log4cxx::Logger::getLogger(name);

	#define LOG_TRACE(stuff) LOG4CXX_TRACE(logger, stuff);
	#define LOG_DEBUG(stuff) LOG4CXX_DEBUG(logger, stuff);
	#define LOG_INFO(stuff) LOG4CXX_INFO(logger, stuff);
	#define LOG_WARN(stuff) LOG4CXX_WARN(logger, stuff);
	#define LOG_ERROR(stuff) LOG4CXX_ERROR(logger, stuff);
	#define LOG_FATAL(stuff) LOG4CXX_FATAL(logger, stuff);

#else
	//cout instead
	#define DEBUG_SHOW(a) cout << #a << "=" << (a) << "\n";
	#define DEBUG_SHOW2(a,b) cout << #a << "=" << (a) << "\t" << #b << "=" << (b) << "\n";
	#define DEBUG_SHOW3(a,b,c) cout << #a << "=" << (a) << "\t" << #b << "=" << (b) << "\t" << #c << "=" << (c) << "\n";
	#define DEBUG_SHOW4(a,b,c,d) cout << #a << "=" << (a) << "\t" << #b << "=" << (b) << "\t" << #c << "=" << (c) << "\t" << #d << "=" << (d) << "\n";
	#define INFO_SHOW(a) cout << #a << "=" << (a) << "\n";
	#define INFO_SHOW2(a,b) cout << #a << "=" << (a) << "\t" << #b << "=" << (b) << "\n";
	#define INFO_SHOW3(a,b,c) cout << #a << "=" << (a) << "\t" << #b << "=" << (b) << "\t" << #c << "=" << (c) << "\n";
	#define INFO_SHOW4(a,b,c,d) cout << #a << "=" << (a) << "\t" << #b << "=" << (b) << "\t" << #c << "=" << (c) << "\t" << #d << "=" << (d) << "\n";
	#define DBG_TRACE

	// NO-OPs
	#define DECLARE_LOGGER
	#define GET_LOGGER(name)

	#define LOG_TRACE(stuff) cout << stuff << "\n";
	#define LOG_DEBUG(stuff) cout << stuff << "\n";
	#define LOG_INFO(stuff) cout << stuff << "\n";
	#define LOG_WARN(stuff) cout << stuff << "\n";
	#define LOG_ERROR(stuff) cout << stuff << "\n";
	#define LOG_FATAL(stuff) cout << stuff << "\n";
#endif


#include <algorithm>

#include <string>
#include <vector>
#include <map>
#include <complex>
#include <assert.h>

#include <boost/serialization/access.hpp>
#include <boost/serialization/split_free.hpp>

#ifdef LOG_ENABLED
	// include log4cxx header files.
	#include "log4cxx/logger.h"
	#include "log4cxx/basicconfigurator.h"
	#include "log4cxx/propertyconfigurator.h"
	#include "log4cxx/helpers/exception.h"
#endif


#include "../alglib/ap.h"
#include "../alglib/interpolation.h"

double const CONST_PI = 3.14159265358979323846;
double const CONV_au_m = 5.2917720859e-11;
double const CONV_au_nm = 5.2917720859e-2;
double const CONV_au_eV = 27.211;
double const CONV_au_s = 2.418884326505e-17;
double const CONV_au_fs = 2.418884326505e-2;
double const CONV_au_V_over_m = 5.1421e11;
double const CONV_au_V = CONV_au_V_over_m * CONV_au_m;

// ------------------------- std::complex<double> addons ----------------------

typedef std::complex<double> dcmplx;

namespace boost { namespace serialization {
template<class Archive>
void save(Archive & ar, const dcmplx & z, const unsigned int version)
{
	double r = real( z );
	double i = imag( z );
    ar & r;
    ar & i;
}
template<class Archive>
void load(Archive & ar, dcmplx & z, const unsigned int version)
{
	double r, i;
    ar & r;
    ar & i;
    z = dcmplx( r, i );
}
}} //namespace boost::serialization
BOOST_SERIALIZATION_SPLIT_FREE(dcmplx)

template <typename T>
int sgn(T val)
{
    return (val > T(0)) - (val < T(0));
}

using std::vector;
using std::string;

namespace liee {

struct Point {
	double x, y;
	Point( double x, double y ) : x(x), y(y) {}
	Point() : x(0), y(0) {}
};

struct less_than_point_x
{
    inline bool operator() (const Point& p1, const Point& p2)
    {
        return ( p1.x  < p2.x );
    }
};

/*
 * Simplistic alternative for ALGLIB's cubic spline.
 */
struct Linear_Interpolant
{
	vector<Point>& d;
	int pos;

	Linear_Interpolant( vector<Point>& data ) : d(data), pos(0)
	{
		std::sort( d.begin(), d.end(), less_than_point_x() );
	}

	double interpol( double x )	{
		if ( x == d.at(pos).x ) return d.at(pos).y;

		// to find the point in our data nearest to the requested x, determine in which direction to move (starting from the cached position of last call)
		int dir = 1;
		if ( x < d.at(pos).x ) { dir = -1; }
		double nearest = 2 * abs( d.front().x - d.back().x );

		size_t i, j = pos;
		while ( i >= 0 && i < d.size() && abs( d.at(i).x - x ) < nearest ) {	// move until the distance to x gets bigger again
			nearest = abs( d.at(i).x - x );
			pos = i;	// cache the index, because next request is probably close to it
			i += dir;
		}
		i = pos;	// last step was one too far
		double xi = d.at(i).x;
		double yi = d.at(i).y;

		if ( x == xi ) return d.at(i).y;

		if ( i == 0 && x < xi ) {	// extrapolate towards left
			double dx = x - xi;
			double m = ( d.at(i+1).y - yi ) / ( d.at(i+1).x - xi );
			return yi + m * dx;
		}

		if ( i == d.size()-1  &&  x > xi ) {	// extrapolate towards right
			double dx = x - xi;
			double m = ( yi - d.at(i-1).y ) / ( xi - d.at(i-1).x );
			return yi + m * dx;
		}

		j = i + 1;
		if ( x < xi ) {			// make sure that: xi < xj  &&  i < j
			j = i;
			i = j - 1;
			xi = d.at(i).x;
			yi = d.at(i).y;
		}
		double xj = d.at(j).x;
		double yj = d.at(j).y;
		// linear interpolation
		return yi + (x-xi) * (yj-yi) / (xj-xi);
	}
};

alglib::spline1dinterpolant to_cubic_spline( vector<Point>& data );

/*! complex error function from product series */
dcmplx cerf( dcmplx z );

/*! real-valued Lambert W-function */
double lambert_w( double x, bool principal = true );

/*!
 * unconstrained unweighed least square fit of the data points.
 * for the form of y = m*x + n, calculate the line parameters (m,n)
 * and their mean deviations from the data (dm, dn)
 */
void linear_fit( const vector<Point>& data, double& m, double& n, double& dm, double& dn );

/*!
 * Random number generator from Numerical Recipes.
 * Recommended for everyday use. The period is about 1.8e19
 */
class Ranq1
{
public:
	/*! current position in the number-loop */		unsigned long long 		v;

	//! Initialise with a constant seed to have repeatable results or with time-stamp or a true random number otherwise.
	Ranq1(unsigned long long seed);

	//! Draw the next random int64.
	inline unsigned long long int64(){
		v ^= v >> 21;
		v ^= v << 35;
		v ^= v >> 4;
		return v * 2685821657736338717LL;
	}
	//! Draw the next random and convert to double.
	inline double doub() {
		return 5.42101086242752217e-20 * int64();
	}

	//! Draw the next and clip to int32.
	inline unsigned int int32() {
		return (unsigned int) int64();
	}
};

struct Except__Preconditions_Fail {
	int specification_code;
	Except__Preconditions_Fail( int c ) : specification_code( c ) {}
};

struct Except__Convergence_Fail {
	int specification_code;
	Except__Convergence_Fail( int c ) : specification_code( c ) {}
};

//----------------------------I/O utility-------------------------------------------------------------

/*!
 * utility function to tar all result files and gzip-compress them before transfer.
 * @see https://github.com/libarchive/libarchive/wiki/Examples
 */
void tar_gz_files( const string& dir_prefix, const vector<string>& files, const string& archive_name );

/*!
 * very minimalist HTTP-GET call, which doesn't care what went wrong if anything
 * unexpected happened and just returns an empty string in that case, the content otherwise.
 * no checks for file-size. it is assumed that the server does not bomb us with a GB of
 * content here.
*/
string get_html_page( const string& host, const string& item, long timeout = 60 );


/*!
 * wraps sprintf("%1.15g", d) to return strings with full double precision
 * TODO this is silly!
 */
string doub2str( double d );

/*!
 * Parses the string representation of an array and appends the values to a given vector of double.
 * @param str     string literal like "{1.0, 3.4, 5}"
 * @param v_out vector for adding the values to
 */
void append_array_literal( const string & str, vector<double> & v_out );

/*!
 * Reads a text file and returns all numbers as vector of double in the order of appearance.
 *
 * Comments starting with "#" are ignored. White spaces are space, tab and comma. Non-numerical
 * parts are ignored.
 */
void parse_datafile( const string filename, vector<double> & data );

double sum( const vector<double> & x );
double arithmetic_mean( const vector<double> & x );
double variance( const vector<double> & x );

/*! Gives the distance between the nearest grid-position: i*step and pos */
double offgrid( double pos, double step );

/* Trapezoid rule summation over sampled data, integration bounds a, b */
double simple_integrate( const vector<Point> data, double a, double b );


//----------------------------very basic juggling-------------------------------------------------------------

template <class T> void sort2( T & a, T & b) {
	if ( a > b ) {
		std::swap( a, b );
	}
}

template <class T> void sort3( T & a, T & b, T & c) {
	sort2( a, b );
	sort2( b, c );
	sort2( a, b );
}

//----------------------------Expression Parser-------------------------------------------------------------

/*!
 * Parser for Infix Expressions
 * Aiming at the simplest definition of a grammar which is still useful and intuitive.
 *
 * BUG: numerals with signed exponents are not supported!
 * 		use 0.001 instead of 1e-3
 * TODO special handling of "-" and "+" operator to prevent their detection inside valid numerals
 *
 * Example:
 * 	( ( sin (-$A) ) + ( 4 * $B ) )
 * 	...evaluates to 'sin(-A) + 4*B' where A and B are in the variables-directory
 *
 * Rules:
 * - every operation is enclosed in brackets ()
 * - every operation has one or two operands and an operator in between them
 * - for single-operand operations, the operator takes first place
 * - whitespace ' ' does not change evaluation and is only for readability
 * - supported operators are: + - * / ^
 * - supported functions are: exp ln sin cos
 * - operands can be c-style numeric constants, nested expressions or variables
 * - the special character $ indicates an operand from the named variables directory
 * - naturally, nested operations are evaluated inside-outwards
 */
class BracketedInfixParser
{
public:
	//!
	BracketedInfixParser( std::map<string, double> * directory_of_variables );
	double evaluate( string expression );
private:
	std::map<string, double> * vars;

	double eval( string ex );
	double eval( string ex, double* operand );
	string strip_white( string s );
};


} //namespace liee

#endif /* MY_UTIL_H_ */
