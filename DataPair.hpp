/* 
 * File:   DataPair.hpp
 * Author: jpcordovae
 *
 * Created on May 26, 2012, 11:21 AM
 */

#ifndef DATAPAIR_HPP
#define DATAPAIR_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cast.hpp>
#include <utility>
#include <deque>

#define IS_UNLIMITED_BUFFER(rule) rule & UNLIMITED_SIZE; // just for compiler test

template<class T=boost::posix_time::ptime, class P=double>
class DataPair {
public:
	
	enum RULES { LIMITED_SIZE   = 0x00,  // keep the last data in memory
					 UNLIMITED_SIZE = 0x01,	 // use all the memory you want
					 NO_SIZE        = 0x02}; // don't save data
					 
	typedef boost::shared_ptr<std::pair<T,P> > pair_ptr;
	
	DataPair(){};
	DataPair(const DataPair& orig);
	
	virtual ~DataPair(){
		m_pair_buffer.clear(); // free used memory
	};
	
	void AddPair(const T &_T, const P &_P){
		{ // for scooped mutex
			boost::mutex::scoped_lock l(m_rules_mtx);
			if(m_rules & LIMITED_SIZE){

				if(m_max_size==0)
				{
					// this shouldn't happend, thir is an errror
					std::cerr << "limited size setted and m_max_size is zero... error";
					return;
				}

				boost::mutex::scoped_lock l(m_pair_buffer_mtx); // I must be caerefull with some many mutex... I think this is bad anywhere
				if(m_pair_buffer.size()==m_max_size)
				{
					m_pair_buffer.pop_front();
					m_pair_buffer.push_back(pair_ptr( new std::pair<T,P>(_T,_P)));
					return;
				}
			}
		}// else, we have unlimited size
		boost::mutex::scoped_lock l(m_pair_buffer_mtx);
		m_pair_buffer.push_back(pair_ptr(new std::pair<T,P>(_T,_P)));
	}
	
	size_t count(void){
		boost::mutex::scoped_lock l(m_pair_buffer_mtx);
		return m_pair_buffer.size();
	}
	
	pair_ptr GetPairAt(size_t _where){
		try
		{
			boost::mutex::scoped_lock l(m_pair_buffer_mtx);
			return m_pair_buffer[_where];
		}catch(std::exception &e)
		{
			std::cerr << e.what();
		}
	}
	
	void SetBufferSize(size_t _max_size){
		boost::mutex::scoped_lock l(m_rules_mtx);
		m_max_size = _max_size;
		m_rules = LIMITED_SIZE;
	}
	
	void clear(void){
		boost::mutex::scoped_lock l(m_pair_buffer_mtx);
		m_pair_buffer.clear();
	}
	
	void UnlimitedSize(){
		boost::mutex::scoped_lock l(m_rules_mtx);
		m_rules = UNLIMITED_SIZE;
		m_max_size = 0;
	}
	
	void SetBufferSizeRules(const typename DataPair<T,P>::RULES _rule){
		boost::mutex::scoped_lock l(m_rules_mtx);
		m_rules = _rule;
		if(m_max_size=0){
			m_max_size = 1000; // just to put some value, can be changed throw SetBufferSize(_size)
		}
	}
	
private:
	
	// rules
	boost::mutex m_rules_mtx;
	RULES m_rules;
	volatile size_t m_max_size;
	// buffer
	boost::mutex m_pair_buffer_mtx;
	std::deque<pair_ptr> m_pair_buffer;
	
};

// try to decipher this operator, :p
template<class T, class P>
typename DataPair<T,P>::RULES operator |(const typename DataPair<T,P>::RULES _a, const typename DataPair<T,P>::RULES _b)
{
	return static_cast<typename DataPair<T,P>::RULES> (static_cast<int>(_a) | static_cast<int>(_b));
}

#endif	/* DATAPAIR_HPP */

