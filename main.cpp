#include "blist.h"

/* --------------- BIDIRECTIONAL LIST PROJECT -----------------------
* Ёто верси€ двусв€зного списка, лично мной написанна€, имеет немного более
* * расширенный функционал, чем мы реализовали на парах.
* * ѕо поводу возникавшей у нас ошибки, она пропала, если работать с объектами, созданными
* * на стеке, то есть без оператора new, € уточню этот вопрос.
* * “аже, мелкие огрехи были. ¬ остальном, мы все правильно сделали.
* * ѕоправил, выполнил рефакторинг. »зучайте, пользуйтесь. */

int main()
{
	setlocale(LC_ALL, "");
	// фигурна€ инициализаци€ двунаправленного списка
	blist <int> b_list{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	// ѕроверка перегрузки оператора квадратных скобок
	size_t i = 0;
	std::cout << "¬веди индекс: ";
	std::cin >> i;

	try
	{
		std::cout << b_list[i] << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	// 1-й вариант использовани€ итератора
	for (auto it = b_list.begin(); it != b_list.end(); ++it)
		std::cout << *it << "; ";

	std::cout << std::endl;

	// 2-й вариант использовани€ итератора
	for (auto&& it : b_list)
		std::cout << it << "; ";
	blist<int>::iterator it = b_list.begin();
	blist<int>::iterator it2 = b_list.end();
	
	std::cout << std::endl;
	std::cout << it[1];
	std::cout << std::endl;
	++it;
	std::cout << *it << std::endl;
	--it;

	std::cout << *it << std::endl;
	it2 = it - 1;
	std::cout << *it2 << std::endl;
	std::cout << *it << std::endl;
	
	it += 2;
	
	std::cout << *it << std::endl;
	//std::cout << *(it -1) << std::endl;
	it -= 1;
	std::cout << *it << std::endl;
	system("pause");
	return 0;
}