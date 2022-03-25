#pragma once
class CTask
{
public:
	CTask();
	CTask(int _value);
	~CTask();
	
	int GetValue() const;

private:
	int m_iValue = 0;
};

