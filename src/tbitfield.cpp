// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len > 0) {
	BitLen = len;
	MemLen = (BitLen + (8 * sizeof(TELEM) - 1)) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
	}
	else throw  std::exception("error: len < 0: TBitField(int len)");
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i<MemLen; i++)
		{
			pMem[i] = bf.pMem[i];
		}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n > BitLen))
		throw std::exception("wrong index");
	else 
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << n % (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n > BitLen)) {
		throw  std::exception("SetBit(const int n)");
	}
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask (n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n > BitLen)) {
		throw std::exception("ClrBit(const int n)");
	}
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask (n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n > BitLen)) {
		throw std::exception("GetBit(const int n)");
	}
	else {
		TELEM temp = GetMemMask(n);
		if((pMem[GetMemIndex(n)]&temp) == 0)
			return 0;
		else
			return 1;
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (BitLen != bf.BitLen)
	{
		delete []pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM [MemLen];
	}

	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res = 1;
	if (BitLen != bf.BitLen)
		res = 0;
	else
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i])
			{
				res = 0;
				break;
			}
			return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (*this == bf)
		return 0;
	return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int maxLen = BitLen;
	if (bf.BitLen > BitLen)
		maxLen = bf.BitLen;

	TBitField temp(maxLen);

	for (int i = 0; i < bf.MemLen; i++)
	{
		temp.pMem[i] = pMem[i]; 
	}

	for (int i = 0; i < MemLen; i++)
	{
		temp.pMem[i] = bf.pMem[i]|temp.pMem[i]; 
	}

	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int maxLen = BitLen;
	if (bf.BitLen > BitLen)
		maxLen = bf.BitLen;

	TBitField temp(maxLen);

	for (int i = 0; i < bf.MemLen; i++)
	{
		temp.pMem[i] = pMem[i]; 
	}
	for (int i = 0; i < MemLen; i++)
	{
		temp.pMem[i] =  bf.pMem[i]&temp.pMem[i]; 		
	}

	return temp;

}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(BitLen);
	for ( int i = 0; i < BitLen; i++)
		if (this->GetBit(i) == 0) temp.SetBit(i);
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	istr >> i;
	while ((i >= 0) && (i < bf.BitLen))
	{
		bf.SetBit(i);
		istr >> i;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i);
	return ostr;
}
