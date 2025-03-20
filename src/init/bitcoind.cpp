// Copyright (c) 2009-2021 The Bitcoin 3.0 Core developers
// Distributed under the MIT software license

#include <interfaces/init.h>
#include <node/context.h>
#include <util/system.h>

class Bitcoin3Init : public interfaces::Init {
public:
    explicit Bitcoin3Init(node::NodeContext& node) : m_node(node) {}
private:
    node::NodeContext& m_node;
};

std::unique_ptr<interfaces::Init> interfaces::MakeNodeInit(node::NodeContext& node, int argc, char* argv[], int& exit_status) {
    return std::make_unique<Bitcoin3Init>(node);
}
