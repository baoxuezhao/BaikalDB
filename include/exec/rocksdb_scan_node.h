// Copyright (c) 2018 Baidu, Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "scan_node.h"
#include "fetcher_node.h"
#include "table_record.h"
#include "table_iterator.h"
#include "transaction.h"
#include "reverse_index.h"
#include "reverse_interface.h"

namespace baikaldb {
class ReverseIndexBase;
class RocksdbScanNode : public ScanNode {
public:
    RocksdbScanNode() {
    }
    virtual ~RocksdbScanNode() {
        for (auto expr : _index_conjuncts) {
            ExprNode::destroy_tree(expr);
        }
        delete _index_iter;
        delete _table_iter;
        if (_reverse_index != nullptr) {
            _reverse_index->clear();
        }
    }
    virtual int init(const pb::PlanNode& node);
    virtual int predicate_pushdown(std::vector<ExprNode*>& input_exprs);
    bool need_pushdown(ExprNode* expr);
    virtual int index_condition_pushdown();
    virtual int open(RuntimeState* state);
    virtual int get_next(RuntimeState* state, RowBatch* batch, bool* eos);
    virtual void close(RuntimeState* state);
    bool contain_condition(ExprNode* expr) {
        std::unordered_set<int32_t> related_tuple_ids;
        expr->get_all_tuple_ids(related_tuple_ids);
        if (related_tuple_ids.size() == 1 && *(related_tuple_ids.begin()) == _tuple_id) {
            return true;
        } 
        return false;
    }
    std::map<int64_t, pb::PossibleIndex>* mutable_region_primary() {
        return &_region_primary;
    }
    void set_related_fetcher_node(FetcherNode* fetcher_node) {
        _related_fetcher_node = fetcher_node;
    }
    FetcherNode* get_related_fetcher_node() const {
        return _related_fetcher_node;
    }
    virtual void transfer_pb(int64_t region_id, pb::PlanNode* pb_node);
    virtual void find_place_holder(std::map<int, ExprNode*>& placeholders) {
        ScanNode::find_place_holder(placeholders);
        for (auto& expr : _index_conjuncts) {
            expr->find_place_holder(placeholders);
        }
    }
private:
    int get_next_by_table_get(RuntimeState* state, RowBatch* batch, bool* eos);
    int get_next_by_table_seek(RuntimeState* state, RowBatch* batch, bool* eos);
    int get_next_by_index_get(RuntimeState* state, RowBatch* batch, bool* eos);
    int get_next_by_index_seek(RuntimeState* state, RowBatch* batch, bool* eos);
    int select_index(RuntimeState* state, const pb::PlanNode& node, std::vector<int>& multi_reverse_index); 
    int choose_index(RuntimeState* state);

private:
    std::vector<int32_t> _field_ids;
    MemRowDescriptor* _mem_row_desc;
    FetcherNode* _related_fetcher_node = NULL;
    SchemaFactory* _factory = nullptr;
    int64_t _index_id = -1;
    int64_t _region_id;
    bool _is_covering_index = true;
    bool _use_get = false;

    //record all used indices here (LIKE & MATCH may use multiple indices)
    std::vector<int64_t> _index_ids;

    // 如果用了排序列做索引，就不需要排序了
    bool _sort_use_index = false;
    bool _scan_forward = true; //scan的方向
    
    //被选择的索引
    std::vector<SmartRecord> _left_records;
    std::vector<SmartRecord> _right_records;
    std::vector<int> _left_field_cnts;
    std::vector<int> _right_field_cnts;
    std::vector<bool> _left_opens;
    std::vector<bool> _right_opens;
    size_t _idx = 0;
    //后续做下推用
    std::vector<ExprNode*> _index_conjuncts;
    IndexIterator* _index_iter = nullptr;
    TableIterator* _table_iter = nullptr;
    ReverseIndexBase* _reverse_index = nullptr;

    TableInfo*       _table_info;
    IndexInfo*       _pri_info;
    IndexInfo*       _index_info;
    pb::RegionInfo*  _region_info;
    std::vector<IndexInfo> _reverse_infos;
    std::vector<std::string> _query_words;
    std::vector<ReverseIndexBase*> _reverse_indexes;
    MutilReverseIndex<CommonSchema> _m_index;

    std::map<int64_t, pb::PossibleIndex> _region_primary;
    std::map<int32_t, int32_t> _index_slot_field_map;
};
}

/* vim: set ts=4 sw=4 sts=4 tw=100 */
