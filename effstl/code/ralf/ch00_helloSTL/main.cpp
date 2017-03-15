#include "extern.h"
#include "item_02.h"

void main()
{
	item *impl = new item_02();

	if (impl) {
		impl->Execute();
	}

	SAFE_DELETE(impl);
}