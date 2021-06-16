#pragma once

/// <summary>
/// Enumerates an array and execute a callback on each entries.
/// </summary>
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InCallback">The callback.</param>
template <typename TEntry>
void RtlArrayForEach(TEntry* InArray, SIZE_T InNumberOfElements, void(*InCallback)(TEntry&))
{
	if (InArray == nullptr)
		return;

	if (InNumberOfElements == 0)
		return;

	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		InCallback(InArray[I]);

/// <summary>
/// Enumerates an array and execute a callback on each entries.
/// </summary>
}
/// <typeparam name="TEntry">The type of the entries in the array.</typeparam>
/// <typeparam name="TContext">The type of the context.</typeparam>
/// <param name="InArray">The array.</param>
/// <param name="InNumberOfElements">The number of elements in the array.</param>
/// <param name="InCallback">The callback.</param>
/// <param name="InContext">The context.</param>
template <typename TEntry, typename TContext = void*>
void RtlArrayForEach(TEntry* InArray, SIZE_T InNumberOfElements, void(*InCallback)(TEntry&, TContext), TContext InContext = nullptr)
{
	if (InArray == nullptr)
		return;

	if (InNumberOfElements == 0)
		return;

	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		InCallback(InArray[I], InContext);
}