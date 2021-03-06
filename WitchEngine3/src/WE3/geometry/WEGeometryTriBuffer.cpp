#include "WEGeometryTriBuffer.h"

#include "WEGeometryTriBufferContainer.h"

namespace WE {


GeometryTriBuffer::Simplification::Simplification() {

	stepLimit = -1;

	mode = M_Planar;
	loopMode = LM_Iterate;

	considerAesthetics = true;
	useIslands = true;

	planarAngle = degToRad(2.5f);
}


void GeometryTriBuffer::destroy() {

	mTris.destroy();
}

void GeometryTriBuffer::append(GeometryVertexBuffer& VB, GeometryIndexBuffer* pIB, const Matrix4x3Base* pTransf) {

	//destroy();

	bool byFace = true;

	if (pIB == NULL) {

		assrt(false);
		return;
	}

	GeometryIndexBuffer& IB = *pIB;

	/*
	bool byFace = (pIB != NULL);

	if (byFace) {

		if (pIB->getElementCount() == 0) {

			byFace = false;
		}
	} 
	
	if (VB.getElementCount() == 0) {

		return;
	}
	*/

	if ((VB.getElementCount() == 0) || (IB.getElementCount() == 0)) {

		return;
	}

	const RenderElementProfile& vprofile = VB.getProfile();
	RenderUINT vertexCount = VB.getElementCount();
	void* vb = VB.bufferPtr();

	RenderSubElIndex el_position = vprofile.findSub(D3DDECLUSAGE_POSITION);

	if (el_position < 0) {

		return;
	}

	float* pos;
	Point points[3];
	
	if (byFace) {

		RenderUINT faceCount = IB.getElementCount() / 3;
		const RenderElementProfile& iprofile = IB.getProfile();
		void* ib = IB.bufferPtr();


		RenderUINT oldCount = mTris.size;
		mTris.resize(oldCount + faceCount);

		TriangleEx1* el = mTris.el;
		RenderUINT indices[3];

		for (RenderUINT f = oldCount, i = 0; f < mTris.size; f++, i += 3) {

			iprofile.face_get(ib, i, indices);
			
			for (int j = 0; j < 3; j++) {

				pos = vprofile.getF(vb, indices[j], el_position);
				el[f].vertices[j].set(MExpand3(pos));
			}

			if (pTransf) {

				pTransf->transformPoint(el[f].vertices[0]);
				pTransf->transformPoint(el[f].vertices[1]);
				pTransf->transformPoint(el[f].vertices[2]);
			}

			el[f].initNormal();
		}
	} 
}

void GeometryTriBuffer::append(GeometryTriBuffer& container) {

	RenderUINT faceCount = container.mTris.size;

	RenderUINT oldCount = mTris.size;
	mTris.resize(oldCount + faceCount);

	for (RenderUINT f = oldCount, cf = 0; f < mTris.size; ++f, ++cf) {

		mTris.el[f] = container.mTris.el[cf];
	}
}

void GeometryTriBuffer::transform(const Matrix4x3Base& transf) {

	for (TriBuffer::Index i = 0; i < mTris.size; ++i) {

		transf.transformPoint(mTris.el[i].vertices[0]);
		transf.transformPoint(mTris.el[i].vertices[1]);
		transf.transformPoint(mTris.el[i].vertices[2]);

		transf.transformVector(mTris.el[i].normal);
	}
}

void GeometryTriBuffer::render(Renderer& renderer, const RenderColor* pColor) {


	/*
	DebugBypass* pBypass = getDebugBypass();

	for (TriBuffer::RenderUINT i = 0; i < mTris.size; ++i) {

		

		if (pBypass->currTri == pBypass->renderTri) {

			renderer.draw(mTris.el[i], &Matrix4x3Base::kIdentity, &RenderColor::kRed, false, false);

		} else {

			renderer.draw(mTris.el[i], &Matrix4x3Base::kIdentity, pColor, false, true);
		}

		++pBypass->currTri;

	}
	*/

	for (TriBuffer::Index i = 0; i < mTris.size; ++i) {

		renderer.draw(mTris.el[i], &Matrix4x3Base::kIdentity, pColor, false, true);
	}
}

void GeometryTriBuffer::addToVol(AAB& vol) {

	for (TriBuffer::Index i = 0; i < mTris.size; ++i) {

		mTris.el[i].addTo(vol);
	}
}

}