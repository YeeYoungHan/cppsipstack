/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#pragma once

// ���� ��ũ�δ� �ʿ��� �ּ� �÷����� �����մϴ�. �ʿ��� �ּ� �÷�����
// ���� ���α׷��� �����ϴ� �� �ʿ��� ����� ���Ե� ���� ���� ������ Windows, Internet Explorer
// ���Դϴ�. �� ��ũ�δ� ������ ���� �̻��� �÷��� �������� ��� ������ ��� ����� Ȱ��ȭ�ؾ�
// �۵��մϴ�.

// �Ʒ� ������ �÷����� �켱�ϴ� �÷����� ������� �ϴ� ��� ���� ���Ǹ� �����Ͻʽÿ�.
// �ٸ� �÷����� ���Ǵ� �ش� ���� �ֽ� ������ MSDN�� �����Ͻʽÿ�.
#ifndef WINVER                          // �ʿ��� �ּ� �÷����� Windows Vista�� �����մϴ�.
#define WINVER 0x0600           // �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_WINNT            // �ʿ��� �ּ� �÷����� Windows Vista�� �����մϴ�.
#define _WIN32_WINNT 0x0600     // �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_WINDOWS          // �ʿ��� �ּ� �÷����� Windows 98�� �����մϴ�.
#define _WIN32_WINDOWS 0x0410 // Windows Me �̻� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_IE                       // �ʿ��� �ּ� �÷����� Internet Explorer 7.0���� �����մϴ�.
#define _WIN32_IE 0x0700        // �ٸ� ������ IE�� �µ��� ������ ������ ������ �ֽʽÿ�.
#endif
