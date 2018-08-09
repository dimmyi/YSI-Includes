// This is quite hard to test, since it can be implemented three different ways
// depending on what exists when compiling.

TEST__ SparseArray()
{
	new
		SparseArray:arr<>;
	ASSERT(!Sparse_Contains(arr, 42));
	Sparse_Set(arr, 42, 101);
	ASSERT(Sparse_Contains(arr, 42));
	ASSERT(Sparse_Get(arr, 42) == 101);
	Sparse_UnSet(arr, 42);
	ASSERT(!Sparse_Contains(arr, 42));
	ASSERT(Sparse_Get(arr, 42) == cellmin);
	new
		BitArray:done<10000>;
	for (new i = 0, idx, val; i != 1000; ++i)
	{
		do
		{
			idx = random(10000);
		}
		while (Bit_Get(done, idx));
		val = random(99999);
		Bit_Let(done, idx);
		ASSERT(!Sparse_Contains(arr, idx));
		Sparse_Set(arr, idx, val);
		ASSERT(Sparse_Contains(arr, idx));
		ASSERT(Sparse_Get(arr, idx) == val);
		val = random(99999);
		Sparse_Set(arr, idx, val);
		ASSERT(Sparse_Contains(arr, idx));
		ASSERT(Sparse_Get(arr, idx) == val);
	}
	foreach (new idx : Bits(done))
	{
		ASSERT(Sparse_Contains(arr, idx));
		Sparse_UnSet(arr, idx);
		ASSERT(Sparse_Get(arr, idx) == cellmin);
		ASSERT(!Sparse_Contains(arr, idx));
	}
}
