#pragma once
#include "thread_pool.h"
#include "GSgame.h"

//
class resource_loader
{
	thread_pool pool_{4};
    std::atomic_int count_{0};

    void (*end_func_)() = nullptr;

	void end()
	{
		if(end_func_!=nullptr&&count_==0)
		{
			end_func_();
            end_func_ = nullptr;
		}
	}

	void post(const std::function<void()> f)
	{
        ++count_;
        pool_.post([this,f]
            {
                f();
                --count_;
                end();
            });
	}
public:	
    void load_mesh(const GSuint id, const char* path)
    {
        post([&] {gsLoadMesh(id, path); });
    }
    void load_skeleton(const GSuint id, const char* path)
    {
        post([&] {gsLoadSkeleton(id, path); });
    }
    void load_animation(const GSuint id, const char* path)
    {
        post([&] {gsLoadAnimation(id, path); });
    }
    void load_octree(const GSuint id, const char* path)
    {
        post([=] {gsLoadOctree(id, path); });
    }

    void on_end(void (*f)())
    {
    	//実行中が無ければ即時
        if (count_ == 0)
            f();
        else
            end_func_ = f;//これ大丈夫？
    }
};
