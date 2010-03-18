#ifndef __PhoenixApplicationStatistics_h__
#define __PhoenixApplicationStatistics_h__

class CApplicationStatistics
{
private:
	int		m_iFPS;
	int     m_iAverageFPS;

public:
	CApplicationStatistics();
	virtual ~CApplicationStatistics();

};

#endif /* __PhoenixApplicationStatistics_h__ */
