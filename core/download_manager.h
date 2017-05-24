#pragma once

#include "queue.h"
#include <thread>
#include <atomic>
#include <memory>
#include <string>

namespace dronelink {

typedef std::function<int(int progress)> progress_callback_t;

class DownloadManager
{
public:
    explicit DownloadManager();
    ~DownloadManager();

    void start();
    void stop();

    bool download_sync(const std::string &url, const std::string &local_path);
    bool download_content_sync(const std::string &url, std::string &content);
    void download_async(const std::string &url, const std::string &local_path,
                        const progress_callback_t &progress_callback = nullptr);

    bool upload_sync(const std::string &target_url, const std::string &local_path);
    void upload_async(const std::string &target_url, const std::string &local_path,
                      const progress_callback_t &progress_callback = nullptr);

    // Non-copyable
    DownloadManager(const DownloadManager &) = delete;
    const DownloadManager &operator=(const DownloadManager &) = delete;

private:

    class WorkItem
    {
    public:
        WorkItem() {}
        virtual ~WorkItem() {}

        WorkItem(WorkItem &) = delete;
        WorkItem operator=(WorkItem &) = delete;
    };

    class DownloadContentItem : public WorkItem
    {
    public:
        DownloadContentItem(const std::string &url) :
            _url(url) { }

        std::string get_url() const
        {
            return _url;
        }

        DownloadContentItem(DownloadContentItem &) = delete;
        DownloadContentItem operator=(DownloadContentItem &) = delete;

    private:
        std::string _url;
    };

    class DownloadItem : public WorkItem
    {
    public:
        DownloadItem(const std::string &url, const std::string &local_path,
                     const progress_callback_t &progress_callback) :
            _url(url),
            _local_path(local_path),
            _progress_callback(progress_callback) { }

        std::string get_local_path() const
        {
            return _local_path;
        }

        std::string get_url() const
        {
            return _url;
        }

        progress_callback_t get_progress_callback() const
        {
            return _progress_callback;
        }

        DownloadItem(DownloadItem &) = delete;
        DownloadItem operator=(DownloadItem &) = delete;

    private:
        std::string _url;
        std::string _local_path;
        progress_callback_t _progress_callback;
    };


    class UploadItem : public WorkItem
    {
    public:
        UploadItem(const std::string &target_url, const std::string &local_path,
                   const progress_callback_t &progress_callback) :
            _target_url(target_url),
            _local_path(local_path),
            _progress_callback(progress_callback) { }

        std::string get_local_path() const
        {
            return _local_path;
        }

        std::string get_target_url() const
        {
            return _target_url;
        }

        progress_callback_t get_progress_callback() const
        {
            return _progress_callback;
        }

        UploadItem(UploadItem &) = delete;
        UploadItem operator=(UploadItem &) = delete;

    private:
        std::string _target_url;
        std::string _local_path;
        progress_callback_t _progress_callback;
    };

    static void work_thread(DownloadManager *self);
    static void do_item(const std::shared_ptr<WorkItem> &item);
    static bool do_download(const std::shared_ptr<DownloadItem> &item);
    static bool do_upload(const std::shared_ptr<UploadItem> &item);

    SafeQueue <std::shared_ptr<WorkItem>> _work_queue {};
    std::thread *_work_thread = nullptr;

    std::atomic<bool> _should_exit {false};
};

} // namespace dronelink

