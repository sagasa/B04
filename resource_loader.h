#pragma once
#include "thread_pool.h"
#include "GSgame.h"
#include <iostream>
//
class resource_loader
{
    thread_pool pool_{ 4 };

    std::function<void()>* end_func_ = nullptr;

    void end()
    {
        if (end_func_ != nullptr && count_ == 0)
        {
            end_func_->operator()();
            end_func_ = nullptr;
        }
    }

    void post(const std::function<void()> f)
    {
        ++count_;
        pool_.post([this, f]
            {
                f();
                --count_;
                end();
            });
    }
public:
    std::atomic_int count_{ 0 };


    void load_mesh(GSuint id, const char* path)
    {
        post([=]
            {
                std::cout << "load mesh " << path << " start" << "\n";
                gsLoadMesh(id, path);
                std::cout << "load mesh " << path << " end" << "\n";
            });
    }
    void load_skeleton(GSuint id, const char* path)
    {
        post([=]
            {
                std::cout << "load skeleton " << path << " start" << "\n";
                gsLoadSkeleton(id, path);
                std::cout << "load skeleton " << path << " end" << "\n";
            });
    }
    void load_animation(GSuint id, const char* path)
    {
        post([=]
            {
                std::cout << "load animation " << path << " start" << "\n";
                gsLoadAnimation(id, path);
                std::cout << "load animation " << path << " end" << "\n";
            });
    }
    void load_octree(GSuint id, const char* path)
    {
        post([=]
            {
                std::cout << "load octree " << path << " start" << "\n";
                gsLoadOctree(id, path);
                std::cout << "load octree " << path << " end" << "\n";
            });
    }

    void on_end(std::function<void()>& f)
    {
        //実行中が無ければ即時
        if (count_ == 0)
            f();
        else
            end_func_ = &f;//これ大丈夫？
    }
};
