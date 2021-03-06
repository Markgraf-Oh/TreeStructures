﻿// TreeStructure.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "BinaryTree.h"
#include "BinarySearchTree.h"
#include "BinaryHeapTree.h"
#include "AVLTreeNode.h"
#include "AVLTree.h"

int main()
{
    AVLTree<int>* tester = new AVLTree<int>(10, [](const int& left, const int& right)->int
        {
            if (left > right) return 1;
            if (left == right) return 0;
            return -1;
        });
    for (int i = 0; i < 20; i ++)
    {
        tester->AddData(i);
    }

    std::cout << tester->GetNodeCount() << std::endl;
    AVLTreeNode<int>* search = nullptr;
    tester->FindData(10, tester->GetRoot(), search);
    std::cout << search->GetHeight() << std::endl;

    for (int i = 0; i < 20; i += 4)
    {
        tester->DeleteData(i);
    }

    std::cout << tester->GetNodeCount() << std::endl;

    delete tester;
    tester = nullptr;

    std::cout << LINE_INFO << std::endl;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
