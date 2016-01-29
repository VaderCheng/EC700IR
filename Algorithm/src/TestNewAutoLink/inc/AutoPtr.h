#ifndef AUTOPTR
#define AUTOPTR

//����ָ����
/* sample

int main() 
{
    AutoPtr<int> p = new int;
    *p = 100;
    printf("%d\n", *p);

    AutoPtr<string> sp = new string;
    *sp = "hello world";
    printf("%s\n", sp->c_str());
    return 0;
}

*/
template<class T>
class AutoPtr 
{
public :
    AutoPtr(T* p = 0) : pointee(p) {} //Ĭ�Ϲ��캯��

    template<class U>
    AutoPtr(AutoPtr<U>& rhs) : pointee(rhs.release()) {}//���ƹ��캯��

    ~AutoPtr() { if(pointee) delete pointee; }

    template<class U>
    AutoPtr<T>& operator=(AutoPtr<U>& rhs) //��ֵ����
	{ 
        if (this != &rhs) 
		{
            reset(rhs.release());
        }
        return *this;
    }

    T& operator*() const {return *pointee;} 
    
    T* operator->() const {return pointee;}

    T* get() const {return pointee;} //��ȡdumb pointer

    T* release()  //�ͷ�dumb pointer ��ӵ��Ȩ����������ֵ
	{
        T* oldPointee = pointee;
        pointee = 0;
        return oldPointee;
    } 
    
    void reset(T* p=0) //�ظ���pָ��
	{ 
        if (pointee != p) 
		{
            if(pointee) delete pointee;
            pointee = p;
        }
    }

private :
    T* pointee;
};

#endif //AUTOPTR



