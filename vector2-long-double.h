#ifndef VECTOR_H

#define VECTOR_H

//struct Vector {long double x;long double y;long double z;};
//Vector p={0,0,0};

class Vector	{

private:
//	long double x,y,z;		// change to public ... 10 may 2014
public:
	long double x,y,z;
	Vector (long double,long double,long double);
	Vector ();
	long double Modulus();
	Vector Hat() const;
	~Vector() {};
	Vector operator/(long double) const;
	Vector operator*(long double) const;
	Vector operator+(Vector) const;
	Vector operator-(Vector) const;
	Vector operator*(Vector) const;
	inline long double get_x() const;
	inline long double get_y() const;
	inline long double get_z() const;

};
Vector::Vector(long double xparam,long double yparam, long double zparam)
	{	x=xparam;y=yparam;z=zparam;		}
Vector::Vector()
	{x=0;y=0;z=0;}
long double Vector::Modulus()
	{
	return std::sqrt(std::pow(x,2) + std::pow(y,2) + std::pow(z,2)    );
	}


Vector Vector::operator/(long double f) const
{
Vector v=*this;
	v.x/=f;
	v.y/=f;
	v.z/=f;
return v;
}
Vector Vector::operator*(long double f) const
{
Vector v=*this;
	v.x*=f;
	v.y*=f;
	v.z*=f;
return v;
}

Vector Vector::operator+(Vector v2) const
{
Vector v=*this;
	v.x+=v2.x;
	v.y+=v2.y;
	v.z+=v2.z;
return v;
}

Vector Vector::operator-(Vector v2) const
{
Vector v=*this;
	v.x-=v2.x;
	v.y-=v2.y;
	v.z-=v2.z;
return v;
}
Vector Vector::operator*(Vector v2) const
{
Vector v;
Vector v1=*this;
	v.x =v1.y*v2.z-v1.z*v2.y;
	v.y =v1.z*v2.x-v1.x*v2.z;
	v.z =v1.x*v2.y-v1.y*v2.x;
return v;
}

Vector Vector::Hat() const
	{
	Vector v=*this;
	long double aaa= v . Modulus();
	v.x/=aaa;
	v.y/=aaa;
	v.z/=aaa;
	return v;
	}

inline long double Vector::get_x() const 
	{
//	Vector v1 = *this;
//	return v1.x;
	return (*this).x;
	}


inline long double Vector::get_y() const 
	{
	return (*this).y;
	}

inline long double Vector::get_z() const 
	{
	return (*this).z;
	}


#endif
