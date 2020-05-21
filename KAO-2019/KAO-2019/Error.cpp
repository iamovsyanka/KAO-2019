#include "pch.h"
#include "Error.h"

namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =																	
	{
		ERROR_ENTRY(0, "Системная ошибка | Недопустимый код ошибки"),											
		ERROR_ENTRY(1, "Системная ошибка | Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "Системная ошибка | Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "Системная ошибка | Превышена длина входного параметра"),
		ERROR_ENTRY(102, "Системная ошибка | Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(103, "Системная ошибка | Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY_NODEF10(110), ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY(200, "Ошибка на этапе лексического анализа | Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(201, "Ошибка на этапе лексического анализа | Превышен размер таблицы лексем"),
		ERROR_ENTRY(202, "Ошибка на этапе лексического анализа | Переполнение таблицы лексем"),
		ERROR_ENTRY(203, "Ошибка на этапе лексического анализа | Превышен размер таблицы идентификаторов"),
		ERROR_ENTRY(204, "Ошибка на этапе лексического анализа | Переполнение таблицы идентификаторов"),
		ERROR_ENTRY(205, "Ошибка на этапе лексического анализа | Неизвестная последовательность символов"),
		ERROR_ENTRY_NODEF(206), ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250),
		ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY(300, "Ошибка на этапе семантического анализа | Не закрыт строковый литерал"),
		ERROR_ENTRY(301, "Ошибка на этапе семантического анализа | Ожидается тип bool!"),
		ERROR_ENTRY(302, "Ошибка на этапе семантического анализа | Отсутствует точка входа Go"),
		ERROR_ENTRY(303, "Ошибка на этапе семантического анализа | Обнаружено несколько точек входа Go"),
		ERROR_ENTRY(304, "Ошибка на этапе семантического анализа | Превышен размер строкового литерала"),
		ERROR_ENTRY(305, "Ошибка на этапе семантического анализа | Объявление переменной без ключевого слова var недопустимо"),
		ERROR_ENTRY(306, "Ошибка на этапе семантического анализа | Необъявленный идентификатор"),
		ERROR_ENTRY(307, "Ошибка на этапе семантического анализа | Недопустимо объявление переменной без указания типа"),
		ERROR_ENTRY(308, "Ошибка на этапе семантического анализа | Попытка реализовать уже существующую функцию"),
		ERROR_ENTRY(309, "Ошибка на этапе семантического анализа | Попытка переопределить формальный параметр функции"),
		ERROR_ENTRY(310, "Ошибка на этапе семантического анализа | Попытка переопределить переменную"),
		ERROR_ENTRY(311, "Ошибка на этапе семантического анализа | Не указан тип функции"),
		ERROR_ENTRY(312, "Ошибка на этапе семантического анализа | Процедура не должна иметь тип"),
		ERROR_ENTRY(313, "Ошибка на этапе семантического анализа | Тип функции и тип возвращаемого значения должны совпадать"),
		ERROR_ENTRY(314, "Ошибка на этапе семантического анализа | Превышено максимально допустимое (3) количество параметров функции"),
		ERROR_ENTRY(315, "Ошибка на этапе семантического анализа | Несовпадение типов передаваемых параметров"),
		ERROR_ENTRY(316, "Ошибка на этапе семантического анализа | Слишком много аргументов в вызове функции"),
		ERROR_ENTRY(317, "Ошибка на этапе семантического анализа | Слишком мало аргументов в вызове функции"),
		ERROR_ENTRY(318, "Ошибка на этапе семантического анализа | Использование пустого строкового литерала недопустимо"),
		ERROR_ENTRY(319, "Ошибка на этапе семантического анализа | Недопустимый целочисленный литерал"),
		ERROR_ENTRY(320, "Ошибка на этапе семантического анализа | Типы данных в выражении не совпадают"),
		ERROR_ENTRY(321, "Ошибка на этапе семантического анализа | Арифметические операторы могут применяться только с целочисленными типами"),
		ERROR_ENTRY(322, "Ошибка на этапе семантического анализа | Логические операторы могут применяться только с целочисленными типами"),
		ERROR_ENTRY_NODEF(323),	ERROR_ENTRY_NODEF(324), ERROR_ENTRY_NODEF(325), ERROR_ENTRY_NODEF(326),
		ERROR_ENTRY_NODEF(327), ERROR_ENTRY_NODEF(328), ERROR_ENTRY_NODEF(329),
		ERROR_ENTRY_NODEF10(330), ERROR_ENTRY_NODEF10(340), ERROR_ENTRY_NODEF10(350), ERROR_ENTRY_NODEF10(360),
		ERROR_ENTRY_NODEF10(370), ERROR_ENTRY_NODEF10(380), ERROR_ENTRY_NODEF10(390),
		ERROR_ENTRY_NODEF100(400),
		ERROR_ENTRY_NODEF10(500), ERROR_ENTRY_NODEF10(510), ERROR_ENTRY_NODEF10(520), ERROR_ENTRY_NODEF10(530), ERROR_ENTRY_NODEF10(540),
		ERROR_ENTRY_NODEF10(550), ERROR_ENTRY_NODEF10(560), ERROR_ENTRY_NODEF10(570), ERROR_ENTRY_NODEF10(580), ERROR_ENTRY_NODEF10(590),
		ERROR_ENTRY(600, "Ошибка на этапе синтаксического анализа | Неверная структура программы"),
		ERROR_ENTRY(601, "Ошибка на этапе синтаксического анализа | Отсутствует список параметров функции при её объявлении"),
		ERROR_ENTRY(602, "Ошибка на этапе синтаксического анализа | Ошибка в параметрах функции при её объявлении"),
		ERROR_ENTRY(603, "Ошибка на этапе синтаксического анализа | Возможно отсутствует тело функции"),
		ERROR_ENTRY(604, "Ошибка на этапе синтаксического анализа | Недопустимое выражение. Ожидаются только литералы и идентификаторы"),
		ERROR_ENTRY(605, "Ошибка на этапе синтаксического анализа | Неверная конструкция в теле функции"),
		ERROR_ENTRY(606, "Ошибка на этапе синтаксического анализа | Ошибка в условном выражении"),
		ERROR_ENTRY(607, "Ошибка на этапе синтаксического анализа | Ошибка в вызове функции"),
		ERROR_ENTRY(608, "Ошибка на этапе синтаксического анализа | Ошибка в арифметическом выражении"),
		ERROR_ENTRY(609, "Ошибка на этапе синтаксического анализа | Ошибка в списке параметров при вызове функции"),
		ERROR_ENTRY(610, "Ошибка на этапе синтаксического анализа | Неверная конструкция в условном выражении"),
		ERROR_ENTRY(611, "Ошибка на этапе синтаксического анализа | Требуется закрывающаяся фигурная скобка"),
		ERROR_ENTRY(612, "Ошибка на этапе синтаксического анализа | Требуется открывающаяся фигурная скобка"),
		ERROR_ENTRY_NODEF(613), ERROR_ENTRY_NODEF(614), ERROR_ENTRY_NODEF(615), ERROR_ENTRY_NODEF(616), ERROR_ENTRY_NODEF(617), ERROR_ENTRY_NODEF(618), ERROR_ENTRY_NODEF(619),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id)
	{
		if (id > 0 && id < ERROR_MAX_ENTRY)
			return errors[id];
		else
			return errors[0];
	}

	ERROR geterrorin(int id, int line, int position)
	{
		ERROR error = geterror(id);
		error.inext.position = position;
		error.inext.line = line;
		return error;
	}
}