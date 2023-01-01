/******************************************************************************
*
*  RecordStorageIO class header
*
*  RecordStorageIO is designed for seamless storage of binary records of
*  arbitary size (max record size limited to 4Gb), accessing records as
*  linked list and reuse space of deleted records. RecordStorageIO uses
*  CachedFileIO to cache frequently accessed data and win IO performance.
*
*  Features:
*    - create/read/update/delete records of arbitrary size
*    - navigate records: first, last, next, previous, exact position
*    - reuse space of deleted records
*    - data consistency check (checksum)
*
*  (C) Boson Database, Bolat Basheyev 2022
*
******************************************************************************/
#pragma once

#include "CachedFileIO.h"

#define _ITERATOR_DEBUG_LEVEL 0

#include <vector>
#include <string>

namespace Boson {

	//----------------------------------------------------------------------------
	// Boson storage header structure (64 bytes)
	//----------------------------------------------------------------------------
	typedef struct {
		uint64_t      signature;           // BOSONDB signature
		uint32_t      version;             // Format version
		uint32_t      endOfFile;           // End of file

		uint64_t      totalRecords;        // Total number of records
		uint64_t      firstDataRecord;     // First data record
		uint64_t      lastDataRecord;      // Last data record

		uint64_t      totalFreeRecords;    // Total number of free records
		uint64_t      firstFreeRecord;     // First free record offset
		uint64_t      lastFreeRecord;      // Last free record offset
	} StorageHeader;

	constexpr uint64_t BOSONDB_SIGNATURE = 0x0042444E4F534F42;
	constexpr uint32_t BOSONDB_VERSION = 0x0001;

	//----------------------------------------------------------------------------
	// Record header structure (40 bytes)
	//----------------------------------------------------------------------------
	typedef struct {
		uint64_t    next;              // Next record position in data file
		uint64_t    previous;          // Previous record position in data file		
		uint64_t    recordID;          // 64-bit unique record ID		
		uint32_t    capacity;          // Record length in bytes including padding
		uint32_t    length;            // Data length in bytes		
		uint32_t    checksum;          // Checksum for data consistency check
		uint32_t    type;              // User defined classification of payload
	} RecordHeader;


	//----------------------------------------------------------------------------
	// RecordStorageIO
	//----------------------------------------------------------------------------
	class RecordStorageIO {
	public:
		RecordStorageIO();
		~RecordStorageIO();

		bool     open(const std::string& dbName, bool readonly = false);
		bool     close();

		uint64_t getTotalRecords();
		uint64_t getTotalFreeRecords();

		bool     setCursor(uint64_t offset);
		uint64_t getCursor();

		bool     first();
		bool     last();
		bool     next();
		bool     previous();

		uint64_t createRecord(const void* data, uint32_t length);
		uint64_t removeRecord();

		uint64_t getID();
		uint64_t setType(uint32_t recordType);
		uint32_t getType();
		uint32_t getLength();
		uint32_t getCapacity();
		uint64_t getNext();
		uint64_t getPrevious();
		uint64_t getData(void* data, uint32_t length);
		uint64_t setData(const void* data, uint32_t length);

	private:

		CachedFileIO  storageFile;
		StorageHeader storageHeader;
		RecordHeader  recordHeader;
		size_t        cursorOffset;
		bool          isReadOnly;

		void     initStorageHeader();
		bool     saveStorageHeader();
		bool     loadStorageHeader();

		uint64_t getRecordHeader(uint64_t offset, RecordHeader& result);
		uint64_t putRecordHeader(uint64_t offset, const RecordHeader& header);

		uint64_t allocateRecord(uint32_t capacity, RecordHeader& result);
		uint64_t createFirstRecord(uint32_t capacity, RecordHeader& result);
		uint64_t appendNewRecord(uint32_t capacity, RecordHeader& result);
		uint64_t getFromFreeList(uint32_t capacity, RecordHeader& result);

		bool     putToFreeList(uint64_t offset);
		void     removeFromFreeList(RecordHeader& freeRecord);

		uint64_t generateID();
		uint32_t checksum(const uint8_t* data, uint64_t length);

	};


}