#include <math.h>
#include <iostream>

#include <list>
using namespace std;

typedef struct _TagPoint3D
{
	double m_x, m_y, m_z;
}Point3D;

class Vector3D{
public:
	Point3D m_point;
public:
	Vector3D(Vector3D&vector);
	Vector3D(double x, double y, double z);
	Vector3D operator=(Vector3D&vector);
	void Set(double x, double y, double z);
	void Get(double &x, double &y, double &z);
	Vector3D Normalize();//��λ��
	Vector3D* CrossProduct(Vector3D &point,Vector3D *retPoint);//���
	double DotProduct(Vector3D &vector);//���
	bool point_is_on_the_vector(Point3D point_0,Point3D point_1);
	     //�жϵ�point_1�Ƿ�����point_0Ϊ����������
	int Left_or_Right(Point3D point_0, Vector3D vector_N, Point3D point_1);
	    //�ж���point_0Ϊ��㣬��vector�ķ��򿴣���point�������������Ҳ�

};

class Polygon3D{
public:
	list<Point3D> m_pointArray;
	Vector3D N;//ƽ�淨����
	int Num;//�������
public:
//	Polygon3D(int Point_Num, Point3D point_array[]);
	Polygon3D();
	Polygon3D(int Point_Num, list<Point3D>&point_array);
	void Removeall( );
	void add_element(Point3D& point);
	bool HasBorderPoint(Point3D&point);
	int HasInnerPoint(Point3D&point);
};

Vector3D::Vector3D(Vector3D&vector)
{
	m_point.m_x=vector.m_point.m_x;
	m_point.m_y=vector.m_point.m_y;
	m_point.m_z=vector.m_point.m_z;
};

Vector3D::Vector3D(double x, double y, double z)
{
	m_point.m_x=x;
	m_point.m_y=y;
	m_point.m_z=z;
};

Vector3D Vector3D::operator =(Vector3D&vector)
{
    m_point = vector.m_point;
	m_point.m_x=vector.m_point.m_x;
	m_point.m_y=vector.m_point.m_y;
	m_point.m_z=vector.m_point.m_z;
	return *this;
};

void Vector3D::Set(double x, double y, double z)
{
	m_point.m_x=x;
	m_point.m_y=y;
	m_point.m_z=z;
};

void Vector3D::Get(double &x, double &y, double &z)
{
	x=this->m_point.m_x;
	y=this->m_point.m_y;
	z=this->m_point.m_z;
};

Vector3D Vector3D::Normalize()
{
	double denominator;
	denominator = sqrt(m_point.m_x*m_point.m_x+m_point.m_y*m_point.m_y+m_point.m_z*m_point.m_z);
	if (denominator == 0)
		return *this;
	else
	{
		m_point.m_x=m_point.m_x/denominator;
	    m_point.m_y=m_point.m_y/denominator;
		m_point.m_z=m_point.m_z/denominator;
	}
	return *this;
};

Vector3D *Vector3D::CrossProduct(Vector3D &point,Vector3D *retPoint)//*this �� point �Ĳ��
{
	double x=0, y=0, z=0;
	x=m_point.m_y*point.m_point.m_z-point.m_point.m_y*m_point.m_z;
	y=point.m_point.m_x*m_point.m_z-m_point.m_x*point.m_point.m_z;
	z=m_point.m_x*point.m_point.m_y-point.m_point.m_x*m_point.m_y;

    retPoint->m_point.m_x = x;
    retPoint->m_point.m_y = y;
    retPoint->m_point.m_z = z;
	return retPoint;
};


double Vector3D::DotProduct(Vector3D &vector)//���
{
	return m_point.m_x*vector.m_point.m_x+m_point.m_y*vector.m_point.m_y+m_point.m_z*vector.m_point.m_z;
};

bool Vector3D::point_is_on_the_vector(Point3D point_0,Point3D point_1)//point_0 is the start point
{
	(*this).m_point = this->Normalize().m_point;
	Vector3D *vector = new Vector3D(0,0,0);
	vector->m_point.m_x = point_1.m_x - point_0.m_x;
	vector->m_point.m_y = point_1.m_y - point_0.m_y;
	vector->m_point.m_z = point_1.m_z - point_0.m_z;
	vector->m_point=vector->Normalize().m_point;
	double Dot=DotProduct(*vector);
	delete vector;
	if((Dot-1<1E-5) && (1-Dot<1E-5))//����������
		return true;
	else
		return false;
};

int Vector3D::Left_or_Right(Point3D point_0, Vector3D vector_N, Point3D point_1)
{
	Vector3D *vector = new Vector3D(0,0,0);
	vector->m_point.m_x = point_1.m_x - point_0.m_x;
	vector->m_point.m_y = point_1.m_y - point_0.m_y;
	vector->m_point.m_z = point_1.m_z - point_0.m_z;

	vector= this->CrossProduct(*vector,vector);//vector����ԭ�����������
	                                  //�����������vector
	double temp= vector_N.DotProduct(*vector);//Ȼ��vector��vector_N�����
	delete vector;
	if( temp>0 )
		return 1;
	else
		return -1;
	//�ж����,���point_1�������Ҳ࣬�ֱ��F1��ֵ1��1
};

Polygon3D::Polygon3D():N(0,0,1){};
//Polygon3D(int Point_Num, CArray<Point3D,Point3D>point_array);
Polygon3D::Polygon3D(int Point_Num, list<Point3D>& point_array):N(0,0,1)
//CArray<Point3D,Point3D>&point_array �˴�����Ϊ����!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
	Num = Point_Num;
	//m_pointArray.Copy(point_array);
	for( int i=0; i<Point_Num; i++ )
	{
		//m_pointArray.Add(point_array[i]);
		list<Point3D>::iterator it=point_array.begin();
        advance(it,i);
		Point3D node;
		node.m_x = (*it).m_x;
		node.m_y = (*it).m_y;
		node.m_z = (*it).m_z;
		m_pointArray.push_back(node);
	}
}

void Polygon3D::add_element(Point3D& point)
{
	Num++;
	//m_pointArray.Add(point);
	Point3D node;
	node.m_x = point.m_x;
	node.m_y = point.m_y;
	node.m_z = point.m_z;
	m_pointArray.push_back(node);
}

void Polygon3D::Removeall()
{
	Num = 0;
	//m_pointArray.RemoveAll();
	m_pointArray.clear();
}

bool Polygon3D::HasBorderPoint(Point3D&point)
//�ȼ��point�Ƿ��Ƕ���εĶ���
//Ȼ����point�Ƿ��ڶ���εı߽���
//�㷨���£�
//��ÿ����P1��P2�������� P1P ������PP2, Ȼ��λ��P1P ��PP2
//Ȼ��Ե�λ����P1P ��PP2�����,�������1���бȽ�
{
	bool check=0;//check��Ϊ����ֵ
	//�����ж�point �Ƿ��Ƕ���εĶ���
	for( int j=0; j<Num; j++ )
	{
		list<Point3D>::iterator it=m_pointArray.begin();
        advance(it,j);
		if((point.m_x==(*it).m_x) &&
			(point.m_y==(*it).m_y) &&
			(point.m_z==(*it).m_z))
			check=true;//�Ƕ���εĶ��㣬�򷵻�true
		return check;
	};

	//������Ƕ���εĶ��㣬���һ���ж�point�Ƿ��ڸ�����
	for( int i=0; i<Num; i++ )
	{
		list<Point3D>::iterator it=m_pointArray.begin();
        advance(it,i);

		Vector3D  *P1= new Vector3D(point.m_x-(*it).m_x,
		               point.m_y-(*it).m_y,
			           point.m_z-(*it).m_z);
		int m;//m��������i+1���ı��
		if (i==Num-1)
			m=0;
		else
			m=i+1;

		list<Point3D>::iterator it0=m_pointArray.begin();
        advance(it0,m);
		Vector3D *P2= new Vector3D((*it0).m_x-point.m_x,
			           (*it0).m_y-point.m_y,
			           (*it0).m_z-point.m_z);
		P1->m_point= P1->Normalize().m_point;
		P2->m_point= P2->Normalize().m_point;
		double Dot=P1->DotProduct(*P2);

		delete P1;
		delete P2;
	//cout<<i<<"   "<<Dot<<endl;
		if((Dot-1<1E-5) && (1-Dot<1E-5))
		{
			std::cout<<i<<"   "<<Dot<<std::endl;
			check=true;
        return check;
		};
	};
	//���check��ֵ,������check
	return check;
};

int Polygon3D::HasInnerPoint(Point3D &point)
{
	int Count = 0;//������Count
	int Index = 0, Index1 = 0, Index2, F1, F2;
	Point3D P1, P2;
	//Vector3D per_L = Vector3D(1,0,0);//����L�ĵ�λ��������
	Vector3D *per_L = new Vector3D(1,0,0);

	if (HasBorderPoint(point))
	{//�жϵ��Ƿ��ڶ������
		//cout<<"���ڶ������"<<endl;
		//return false;
		delete per_L;
		return 0;
	};
	for ( int i=0; i<Num; i++ )
	{//������һ�����������ϵĵ�
		list<Point3D>::iterator it=m_pointArray.begin();
        advance(it,i);
		if( !per_L->point_is_on_the_vector(point, (*it)))
		{
			Index = i;
			Index1 = i;
			P1 = (*it);
			break;
		}
		else
			continue;
	};
	//�ж�P1��Per_L���ߵ������Ҳ�
	//////////////////////////////////////////////////////////////

	F1=per_L->Left_or_Right(point, N, P1);//ֱ�ӵ��ú���
	//cout<<F1<<endl;

	for( int j=Index+1; j<Num+Index+1; j++ )//??????
	{//������ѭ������ÿ���߷ֱ��ж�
		int m=(j % Num);
		list<Point3D>::iterator it=m_pointArray.begin();
        advance(it,m);
		P2=(*it);
		//�ж�P2�Ƿ�������per_L�ϣ�����ǣ���ת�´�ѭ��
		if( per_L->point_is_on_the_vector(point, P2))
			continue;
		else//������������ϣ����ж�P2�������Ĳ�,������P2���±�
		{
			F2=per_L->Left_or_Right(point, N, P2);
			Index2= j;
		};
		if( F1*F2==-1 )
		{
			if (Index2-Index1>1) // || (Index2-Index1<-1))
				Count++;
			else
			{
				double y=point.m_y;
				double x=(y-P1.m_y)*(P2.m_x-P1.m_x)/(P2.m_y-P1.m_y)+P1.m_x;
				//Vector3D vector_temp=Vector3D(x-point.m_x, y-point.m_y, 0);
				//if( per_L.DotProduct(vector_temp)>0 )
				if( x > point.m_x )
					Count++;
			};
		};
		Index1=Index2;
		P1=P2;
		F1=F2;
	}
	delete per_L;
	if(Count%2)
	{
		//cout<<"���ڶ������"<<endl;
		return 1;
	}
	else
	{
		//cout<<"���ڶ������"<<endl;
		return 2;
	}
	/////////////////////////////////////////////////////////////////
};

Polygon3D Polygon_result;//ȫ�ֱ���
