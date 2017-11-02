#include "RetroSnaker.h"
#include <thread>
void thread1()
{
	for (int i = 0; i < 1000; i++)
	{
	printf("th1 = %d\n", i);
	}
}
void thread2()
{
	for (int i = 0; i < 1000; i++)
	{
		printf("th2 = %d\n", i);
	}
}

void main()
{
	/*std::thread th1(thread1);

	std::thread th2(thread2);
	th2.join();
	th1.join();*/
	RetroSnaker::getInstance()->initGame();

}