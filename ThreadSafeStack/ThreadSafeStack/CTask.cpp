#include "CTask.h"

CTask::CTask()
{
}

CTask::CTask(int _value)
{
    m_iValue = _value;
}

CTask::~CTask()
{
}

int CTask::GetValue() const
{
    return m_iValue;
}
