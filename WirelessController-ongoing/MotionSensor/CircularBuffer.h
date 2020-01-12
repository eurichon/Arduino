/* 
* CircularBuffer.h
*
* Created: 22/6/2019 6:31:06 μμ
* Author: evrichon
*/


#ifndef __CIRCULARBUFFER_H__
#define __CIRCULARBUFFER_H__

template <class XType = float>class CircularBuffer
{
	private:
	//integers
	int buffer_size;
	int pos_to_add;
	int pos_to_view;
	//booleans
	bool is_filled;
	bool parsed_once;
	//floats
	float moving_aver;
	//tables
	XType *buffer;
	public:
	CircularBuffer(int size);
	bool isFilled();
	bool isParsed();
	void addElement(XType element);
	void resetReading();
	float getMovAver();
	//void printDetailes();
	XType getNextElement();
	XType getPreElement();
	XType getIelement(int i);
	~CircularBuffer();
};


template <class XType>
CircularBuffer<XType>::CircularBuffer(int size)
{
	this->buffer_size = size;
	this->is_filled = false;
	this->parsed_once = false;
	this->pos_to_add = 0;
	this->pos_to_view = 0;
	this->moving_aver = 0;

	buffer = new XType[this->buffer_size];

	register int i;
	for (i = 0;i < size; ++i)
		buffer[i] = 0;
}

template<class XType>
bool CircularBuffer<XType>::isFilled()
{
	return this->is_filled;
}

template<class XType>
bool CircularBuffer<XType>::isParsed()
{
	return this->parsed_once;
}

template<class XType>
void CircularBuffer<XType>::addElement(XType element)
{
	XType temp = this->buffer[pos_to_add];
	this->buffer[pos_to_add] = element;
	if (pos_to_add == (buffer_size - 1))
		is_filled = true;
	this->pos_to_add = (++pos_to_add) % buffer_size;
  this->parsed_once = false;
  
  
  //moving average
  if(!this->isFilled()){
  	this->moving_aver = moving_aver + element;
  }else{
  	this->moving_aver = moving_aver + (element - (float)temp);
  }
  	
}


template<class XType>
float CircularBuffer<XType>::getMovAver(){
	if(this->isFilled()){
		return this->moving_aver/(buffer_size-1);
	}else
		return 0;
}



template<class XType>
void CircularBuffer<XType>::resetReading()
{
	this->pos_to_view = (this->pos_to_add);
}
/*
template<class XType>
void Cbuffer<XType>::printDetailes()
{
	std::cout << "<>==========================================<>" << std::endl;
	std::cout << "The size of the buffer is: " << this->buffer_size << std::endl;
	std::cout << "Is filled: " << this->is_filled << std::endl;
	std::cout << "Position of the current element that has been added: " << this->pos_to_add << std::endl;
	std::cout << "Position of the current element that has been read: " << this->pos_to_view << std::endl;
	std::cout << "Is the buffer parsed during this reading cycle: " << this->parsed_once << std::endl;
	std::cout << "The table contains the elements: ";
	for (register int i = 0; i < this->buffer_size;++i)
	{
		if(i ==this->pos_to_add && i==this->pos_to_view)
			std::cout << this->buffer[i] << "-ADDED -VIEWED, ";
		else if(i == this->pos_to_add)
			std::cout << this->buffer[i] << "-ADDED, ";
		else if (i == this->pos_to_view)
			std::cout << this->buffer[i] << "-VIEWED, ";
		else if (i == (buffer_size - 1))
			std::cout << this->buffer[i] << ".";
		else
			std::cout << this->buffer[i] << ", ";
	}
	std::cout << "<>==========================================<>" << std::endl;
}
*/
template<class XType>
XType CircularBuffer<XType>::getNextElement()
{
  int temp = pos_to_view;
	this->pos_to_view = (++this->pos_to_view) % buffer_size;
	
	if (pos_to_view == pos_to_add)
		parsed_once = true;
	else
		parsed_once = false;
  return this->buffer[temp];

}


template<class XType>
XType CircularBuffer<XType>::getPreElement()
{
  int temp = this->pos_to_add;
  if(temp == 0){
    temp = buffer_size - 1;
  }else{
    temp = temp - 1;
  }
  return this->buffer[temp];

}




template<class XType>
XType CircularBuffer<XType>::getIelement(int i)
{
	return this->buffer[i];
}

template <class XType> CircularBuffer<XType>::~CircularBuffer()
{
	delete this->buffer;
}


#endif //__CIRCULARBUFFER_H__
