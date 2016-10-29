/*
 * DB_Manager.h
 *
 *  Created on: Oct 27, 2016
 *      Author: zhangyalei
 */

#ifndef DB_MANAGER_H_
#define DB_MANAGER_H_

#include "boost/unordered_map.hpp"
#include "DB_Operator.h"
#include "Object_Pool.h"

#define MAX_RECORD_NUM 200000

class DB_Manager {
public:
	typedef boost::unordered_map<int, DB_Operator *> DB_Operator_Map;
	typedef boost::unordered_map<int64_t, Block_Buffer *> RECORD_BUFFER_MAP; //数据库记录集合
	typedef boost::unordered_map<std::string, RECORD_BUFFER_MAP *> TABLE_BUFFER_MAP; //表名集合
	typedef boost::unordered_map<int, TABLE_BUFFER_MAP *> DB_BUFFER_MAP; //数据库ID集合
	typedef boost::unordered_map<int64_t, Block_Buffer *> TEMP_DATA_MAP;
	typedef Object_Pool<Block_Buffer, Thread_Mutex> Buffer_Pool;
public:
	static DB_Manager *instance(void);

	int init_db_operator();
	DB_Operator *db_operator(int type);
	void save_data(int db_id, DB_Struct *db_struct, Block_Buffer *buffer, int flag);
	int load_data(int db_id, DB_Struct *db_struct, int64_t index, std::vector<Block_Buffer *> &buffer_vec);
	int delete_data(int db_id, DB_Struct *db_struct, Block_Buffer *buffer);

	void set_data(int64_t index, DB_Struct *db_struct, Block_Buffer *buffer);
	Block_Buffer *get_data(int64_t index, DB_Struct *db_struct);
	void clear_data(int64_t index);

	inline Block_Buffer *pop_buffer(){return buffer_pool_.pop();};
	inline void push_buffer(Block_Buffer *buffer){buffer->reset();buffer_pool_.push(buffer);};

private:
	DB_Manager(void);
	virtual ~DB_Manager(void);
	DB_Manager(const DB_Manager &);
	const DB_Manager &operator=(const DB_Manager &);

	RECORD_BUFFER_MAP *get_record_map(int db_id, std::string table_name);

private:
	static DB_Manager *instance_;
	DB_Operator_Map db_operator_map_;
	DB_BUFFER_MAP db_buffer_map_;
	TEMP_DATA_MAP temp_data_map_;
	Buffer_Pool buffer_pool_;
};

#define DB_MANAGER 	DB_Manager::instance()
#define DB_OPERATOR(db_id) DB_MANAGER->db_operator((db_id))

#endif /* DB_MANAGER_H_ */