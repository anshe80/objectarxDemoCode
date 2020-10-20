#pragma once

using namespace System;
using namespace Autodesk::AutoCAD::Geometry;
using namespace Autodesk::AutoCAD::DatabaseServices;

class ZfgkTriangle;

namespace Zfgk
{
	namespace TriangleEntityDB
	{
		// ����һ����Entity�̳е��й��࣬���з�װ��ZfgkTriangle��Ķ������
		[Autodesk::AutoCAD::Runtime::Wrapper("ZfgkTriangle")]
		public __gc class MgTriangle :  public Autodesk::AutoCAD::DatabaseServices::Entity
		{
		public:
			MgTriangle(void);
			MgTriangle(Point3d pt1, Point3d pt2, Point3d pt3);

		public private:
			MgTriangle(System::IntPtr unmanagedPointer, bool autoDelete);

			// ��÷��й�ʵ��ָ��
			inline ZfgkTriangle* GetImpObj()
			{
				// UnmanagedObject()��һ��AutoCAD�йܰ�װ���������ṩ��һ��ֱ�ӷ���VC++��ARXʵ��ķ���
				return static_cast<ZfgkTriangle*>(UnmanagedObject.ToPointer());
			}

		public:
			// ������ж���
			void GetVerts([Runtime::InteropServices::Out] Autodesk::AutoCAD::Geometry::Point3dCollection*& verts);

			// ����ĳ�������λ��
			void SetVertAt(int index, Point3d point);

			// ���������
			__property double get_Area();
		};
	}
}
