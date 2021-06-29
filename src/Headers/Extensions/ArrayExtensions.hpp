#pragma once

/// <summary>
/// Enumerates an array and execute a callback on each entries.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InCallback">The callback.</param>
template <typename TEntry>
void RtlArrayForEach(TEntry* InArray, SIZE_T InNumberOfElements, void(*InCallback)(SIZE_T, TEntry&))
{
	if (InArray == nullptr)
		return;

	if (InNumberOfElements == 0)
		return;

	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		InCallback(I, InArray[I]);
}

/// <summary>
/// Enumerates an array and execute a callback on each entries.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <typeparam name="TContext">The type of the context.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TEntry, typename TContext = void*>
void RtlArrayForEach(TEntry* InArray, SIZE_T InNumberOfElements, TContext InContext, void(*InCallback)(SIZE_T, TEntry&, TContext))
{
	if (InArray == nullptr)
		return;

	if (InNumberOfElements == 0)
		return;

	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		InCallback(I, InArray[I], InContext);
}

/// <summary>
/// Enumerates an array and execute a comparer callback on each entries.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InCallback">The callback.</param>
template <typename TEntry>
BOOLEAN RtlArrayMatchAll(CONST TEntry* InArray, SIZE_T InNumberOfElements, bool(*InCallback)(CONST TEntry))
{
	struct ARRAY_CONTEXT
	{
		bool(*Callback)(CONST TEntry);
		BOOLEAN MatchAll;
	};

	ARRAY_CONTEXT Context;
	Context.Callback = InCallback;
	Context.MatchAll = InNumberOfElements != 0 ? TRUE : FALSE;
	
	RtlArrayForEach<TEntry, PVOID>((TEntry*) InArray, InNumberOfElements, &Context, [] (SIZE_T InIndex, TEntry& InEntry, PVOID InContext) -> void
	{
		auto* Context = (ARRAY_CONTEXT*) InContext;

		if (Context->Callback(InEntry) == FALSE)
			Context->MatchAll = FALSE;
	});
	
	return Context.MatchAll;
}

/// <summary>
/// Enumerates an array and execute a comparer callback on each entries.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InComparedValue">The compared value.</param>
template <typename TEntry>
BOOLEAN RtlArrayMatchAll(CONST TEntry* InArray, SIZE_T InNumberOfElements, CONST TEntry InComparedValue)
{
	struct ARRAY_CONTEXT
	{
		TEntry ComparedValue;
		BOOLEAN MatchAll;
	};

	ARRAY_CONTEXT Context;
	Context.ComparedValue = InComparedValue;
	Context.MatchAll = InNumberOfElements != 0 ? TRUE : FALSE;
	
	RtlArrayForEach<TEntry, PVOID>((TEntry*) InArray, InNumberOfElements, &Context, [] (SIZE_T InIndex, TEntry& InEntry, PVOID InContext) -> void
	{
		auto* Context = (ARRAY_CONTEXT*) InContext;

		if (InEntry != Context->ComparedValue)
			Context->MatchAll = FALSE;
	});
	
	return Context.MatchAll;
}
