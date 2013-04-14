#ifndef ZHJSMARTPOITER_H
#define ZHJSMARTPOITER_H

#include <assert.h>
template <typename T>
class ZhjSmartPointer {
public:
	ZhjSmartPointer();
	explicit ZhjSmartPointer(T *ptr);

	ZhjSmartPointer(const ZhjSmartPointer &smartPtr);
	ZhjSmartPointer &operator =(const ZhjSmartPointer &smartPtr);
	~ZhjSmartPointer();
	template <typename OTHER> ZhjSmartPointer(const ZhjSmartPointer<OTHER> &smartPtr) {
		ptr_ = (T *)smartPtr.ptr();
		refCnt_ = smartPtr.refCnt();
		if (ptr_) {
			(*refCnt_)++;
		}
	}

	template <typename OTHER> ZhjSmartPointer &operator =(const ZhjSmartPointer<OTHER> &smartPtr) {
		deletePtr();

		ptr_ = (T *)smartPtr.ptr();
		refCnt_ = smartPtr.refCnt();
		if (ptr_) {
			(*refCnt_)++;
		}
		
		return *this;
	}
	operator bool() const;

	T &operator *() const;
	T *operator ->() const;

	bool operator ==(const ZhjSmartPointer &smartPtr) const;
	bool operator !=(const ZhjSmartPointer &smartPtr) const;

	T *ptr() const;
	size_t *refCnt() const;
	void reset(T *ptr);
	
private:
	void copyPtr(const ZhjSmartPointer &smartPtr);
	void deletePtr();
	T *ptr_;
	size_t *refCnt_;
};

template <typename T>
ZhjSmartPointer<T>::ZhjSmartPointer()
{
	ptr_ = NULL;
	refCnt_ = NULL;
}

template <typename T>
ZhjSmartPointer<T>::ZhjSmartPointer(T *ptr)
{
	ptr_ = ptr;
	if (ptr_) {
		refCnt_ = new size_t;
		*refCnt_ = 1;
	} else {
		refCnt_ = NULL;
	}
}

template <typename T>
ZhjSmartPointer<T>::ZhjSmartPointer(const ZhjSmartPointer &smartPtr)
{
	copyPtr(smartPtr);
}

template <typename T>
ZhjSmartPointer<T> &ZhjSmartPointer<T>::operator =(const ZhjSmartPointer &smartPtr)
{
	if (this != &smartPtr) {
		deletePtr();
		copyPtr(smartPtr);
	} else {
		// self assignment, do nothing
	}

	return *this;
}

/*
template <typename T, typename OTHER>
ZhjSmartPointer<T>::ZhjSmartPointer(const ZhjSmartPointer<OTHER> &smartPtr)
{
	ptr_ = (T *)smartPtr.ptr_;
	refCnt_ = smartPtr.refCnt_;
	if (ptr_) {
		(*refCnt_)++;
	}
}

template <typename T, typename OTHER>
ZhjSmartPointer<T> &ZhjSmartPointer<T>::operator =(const ZhjSmartPointer<OTHER> &smartPtr)
{
	deletePtr();

	ptr_ = (T *)smartPtr.ptr_;
	refCnt_ = smartPtr.refCnt_;
	if (ptr_) {
		(*refCnt_)++;
	}
}
*/
template <typename T>
ZhjSmartPointer<T>::~ZhjSmartPointer()
{
	deletePtr();
}


template <typename T>
void ZhjSmartPointer<T>::copyPtr(const ZhjSmartPointer &smartPtr)
{
	ptr_ = smartPtr.ptr_;
	refCnt_ = smartPtr.refCnt_;
	if (ptr_) {
		(*refCnt_)++;
	}
}

template <typename T>
void ZhjSmartPointer<T>::deletePtr()
{
	if (ptr_) {
		(*refCnt_)--;
		if (*refCnt_ == 0) {
			delete ptr_;
			delete refCnt_;
		}
	}
}

template <typename T>
ZhjSmartPointer<T>::operator bool() const
{
	return ptr_ != NULL;
}

template <typename T>
T &ZhjSmartPointer<T>::operator *() const
{
	return *ptr_;
}

template <typename T>
T *ZhjSmartPointer<T>::operator ->() const
{
	return ptr_;
}

template <typename T>
bool ZhjSmartPointer<T>::operator ==(const ZhjSmartPointer &smartPtr) const
{
	return ptr_ == smartPtr.ptr_;
}

template <typename T>
bool ZhjSmartPointer<T>::operator !=(const ZhjSmartPointer &smartPtr) const
{
	return ptr_ != smartPtr.ptr_;
}

template <typename T>
T *ZhjSmartPointer<T>::ptr() const
{
	return ptr_;
}

template <typename T>
size_t *ZhjSmartPointer<T>::refCnt() const
{
	assert(ptr_);
	return refCnt_;
}

template <typename T>
void ZhjSmartPointer<T>::reset(T *ptr)
{
	deletePtr();
	
	ptr_ = ptr;
	if (ptr_) {
		refCnt_ = new size_t;
		*refCnt_ = 1;
	} else {
		refCnt_ = NULL;
	}
}

#endif
