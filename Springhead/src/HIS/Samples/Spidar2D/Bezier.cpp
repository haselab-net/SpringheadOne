
#include <windows.h>
#include "Graph.h"

#include <conio.h>

/////////////////////////////////////////////////////////////////////////////
//
//  �O���[�o���ϐ�
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
		{		//	�L�[����
			int ch = getch();
			switch(ch)
			{
				case 'g':		//	�L�����u���[�V����
					{
						p_graph.Window_Create_Plot();
					break;
					}
				case 0x1b:		//	ESC:�I��
					goto end;
					break;
			}
		}
	}
	end:;
	return 0;
	
}
