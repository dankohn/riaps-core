//
// Created by istvan on 3/27/19.
//

#ifndef RIAPS_CORE_R_SAFE_MAP_H
#define RIAPS_CORE_R_SAFE_MAP_H

#include <unordered_map>
#include <utils/r_timeout.h>
#include <mutex>

class SafeMap : protected std::unordered_map<std::string, riaps::utils::Timeout<std::chrono::milliseconds>> {
public:
    SafeMap() = default;
    SafeMap(const SafeMap&) = delete;

    bool contains(const std::string& node_id) {
        std::lock_guard<std::mutex> lock(mut_);
        return this->find(node_id) != this->end();
    }

    uint32_t size() {
        std::lock_guard<std::mutex> lock(mut_);
        return this->size();
    }

    std::shared_ptr<std::set<std::string>> keys() {
        std::lock_guard<std::mutex> lock(mut_);
        auto result = std::make_shared<std::set<std::string>>();
        for (auto& n : *this){
            if (n.second.IsTimeout()) continue;
            result->emplace(n.first);
        }
        return result;
    }

    /**
     * returns if any of the known nodes has timed out (haven't heard from)
     */
    bool AnyTimeout() {
        std::lock_guard<std::mutex> lock(mut_);
        for (auto& n : *this){
            if (n.second.IsTimeout()) return true;
        }
        return false;
    }

    uint32_t DeleteTimedOut() {
        std::lock_guard<std::mutex> lock(mut_);
        uint32_t deleted=0;
        for(auto it = this->begin(); it != this->end();)
        {
            if ((*it).second.IsTimeout())
            {
                it = this->erase(it);
                deleted++;
            }
            else
                ++it;
        }
        return deleted;
    }

    riaps::utils::Timeout<std::chrono::milliseconds>* timeout(const std::string& node_id) {
        if (!contains(node_id)) return nullptr;
        std::lock_guard<std::mutex> lock(mut_);
        return &this->at(node_id);
    }
    void put(const std::string& node_id, riaps::utils::Timeout<std::chrono::milliseconds>& timeout) {
        std::lock_guard<std::mutex> lock(mut_);
        this->insert(make_pair(node_id, timeout));
    }

    ~SafeMap() = default;
private:
    std::mutex mut_;
};
#endif //RIAPS_CORE_R_SAFE_MAP_H
