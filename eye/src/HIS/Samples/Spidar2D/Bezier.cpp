
#include <windows.h>
#include "Graph.h"

#include <conio.h>

/////////////////////////////////////////////////////////////////////////////
//
//  グローバル変数
//

extern double x_target_point;
extern double y_target_point;
main()
{

	while(1)
	{
		static Graph p_graph;
		x_target_point=100;
		y_target_point=100;
		if (kbhit())
		{		//	キー入力
			int ch = getch();
			switch(ch)
			{
				case 'g':		//	キャリブレーション
					{
						p_graph.Window_Create_Plot();
					break;
					}
				case 0x1b:		//	ESC:終了
					goto end;
					break;
			}
		}
	}
	end:;
	return 0;
	
}
