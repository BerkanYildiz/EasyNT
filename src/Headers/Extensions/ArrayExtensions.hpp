#pragma once

/// <summary>
/// Enumerates an array and execute a callback on each entries.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InCallback">The callback.</param>
template <typename TEntry>
void CkArrayForEach(TEntry* InArray, SIZE_T InNumberOfElements, void(*InCallback)(SIZE_T, TEntry&))
{
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
template <typename TEntry, typename TContext>
void CkArrayForEach(TEntry* InArray, SIZE_T InNumberOfElements, TContext InContext, void(*InCallback)(SIZE_T, TEntry&, TContext))
{
	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		InCallback(I, InArray[I], InContext);
}

/// <summary>
/// Enumerates an array and check if every entries match the comparer.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InCallback">The comparer.</param>
/// <returns>True if array is not empty and every entries match the condition, false otherwise.</returns>
template <typename TEntry>
BOOLEAN CkArrayMatchAll(CONST TEntry* InArray, SIZE_T InNumberOfElements, bool(*InCallback)(TEntry))
{
	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		if (!InCallback(InArray[I]))
			return false;

	return InNumberOfElements != 0 ? true : false;
}

/// <summary>
/// Enumerates an array and check if every entries match the comparer.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <typeparam name="TContext">The type of the context.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InContext">The number of elements in the array.</param>
/// <param name="InNumberOfElements">The context.</param>
/// <param name="InCallback">The comparer.</param>
/// <returns>True if array is not empty and every entries match the condition, false otherwise.</returns>
template <typename TEntry, typename TContext>
BOOLEAN CkArrayMatchAll(CONST TEntry* InArray, SIZE_T InNumberOfElements, TContext InContext, bool(*InCallback)(TEntry, TContext))
{
	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		if (!InCallback(InArray[I], InContext))
			return false;

	return InNumberOfElements != 0 ? true : false;
}

/// <summary>
/// Enumerates an array and check if every entries match the comparer.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InComparedValue">The compared value.</param>
/// <returns>True if array is not empty and every entries match the condition, false otherwise.</returns>
template <typename TEntry>
BOOLEAN CkArrayMatchAll(CONST TEntry* InArray, SIZE_T InNumberOfElements, CONST TEntry InComparedValue)
{
	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		if (InArray[I] != InComparedValue)
			return false;

	return InNumberOfElements != 0 ? true : false;
}

/// <summary>
/// Enumerates an array and check if at least 1 entry match the comparer.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InCallback">The comparer.</param>
/// <returns>True if array is not empty and at least 1 entry match the condition, false otherwise.</returns>
template <typename TEntry>
BOOLEAN CkArrayMatchAny(CONST TEntry* InArray, SIZE_T InNumberOfElements, bool(*InCallback)(TEntry))
{
	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		if (InCallback(InArray[I]))
			return true;

	return false;
}

/// <summary>
/// Enumerates an array and check if at least 1 entry match the comparer.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <typeparam name="TContext">The type of the context.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InContext">The number of elements in the array.</param>
/// <param name="InCallback">The comparer.</param>
/// <returns>True if array is not empty and at least 1 entry match the condition, false otherwise.</returns>
template <typename TEntry, typename TContext>
BOOLEAN CkArrayMatchAny(CONST TEntry* InArray, SIZE_T InNumberOfElements, TContext InContext, bool(*InCallback)(TEntry, TContext))
{
	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		if (InCallback(InArray[I], InContext))
			return true;

	return false;
}

/// <summary>
/// Enumerates an array and check if at least 1 entry match the comparer.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InComparedValue">The compared value.</param>
/// <returns>True if array is not empty and at least 1 entry match the condition, false otherwise.</returns>
template <typename TEntry>
BOOLEAN CkArrayMatchAny(CONST TEntry* InArray, SIZE_T InNumberOfElements, CONST TEntry InComparedValue)
{
	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		if (InArray[I] == InComparedValue)
			return true;
	
	return false;
}

/// <summary>
/// Reverse the order of the values inside an array.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
template <typename TEntry>
VOID RtlArrayReverse(TEntry* InArray, SIZE_T InNumberOfElements)
{
	if (InArray == nullptr)
		return;

	if (InNumberOfElements == 0)
		return;

	for (SIZE_T I = 0; I < InNumberOfElements / 2u; I++)
	{
		CONST TEntry PreviousValue = InArray[I];
		InArray[I] = InArray[InNumberOfElements - I - 1];
		InArray[InNumberOfElements - I - 1] = PreviousValue;
	}
}
