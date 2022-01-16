#include <cstdio>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sys/ioctl.h>
#include <chrono>
#include <exception>
#include <thread>
#include <filesystem>

#include <fmt/format.h>

#include "gpio_chardev_ioctl.h"


constexpr uint32_t gpioNumber(uint32_t port, uint32_t bit){ return (32 * (port) + (bit)); }

constexpr auto kLedGreen {gpioNumber(11, 10)};
constexpr auto kLedRed {gpioNumber(0, 17)};
constexpr auto kButton { gpioNumber(0, 6)};

struct FileRaiiGuard {
    FileRaiiGuard(std::string_view filePath):m_fileHandle{}
    {
        if (!std::filesystem::exists(filePath)) {
            throw std::runtime_error(
                        "Bad file path for the FileRaiiGuard");
        }
        m_fileHandle = open(filePath.data(), O_WRONLY);

        if (m_fileHandle == kInvalidFileHandle) {
            throw std::runtime_error(
                        "Failed to open the given file");
        }
    }
    FileRaiiGuard &operator=(const FileRaiiGuard &) = delete;
    FileRaiiGuard(const FileRaiiGuard &) = delete;

    ~FileRaiiGuard()
    {
        if (m_fileHandle != kInvalidFileHandle) {
            close(m_fileHandle);
        }
    }

public:
    template<typename TIOCTLCall, typename  IOCtlData>
    void passIoctl(TIOCTLCall&& ioctlCall, IOCtlData&& data)
    {
        auto returnCode = ioctl(m_fileHandle, ioctlCall,&data);
        if(returnCode == kInvalidIoctlCall)
            throw std::runtime_error(fmt::format("IOCL to device finished with the error:{}",returnCode));
    }

private:
    static constexpr inline std::int32_t kInvalidIoctlCall = -1;
    static constexpr inline std::int32_t kInvalidFileHandle = -1;

private:
    int m_fileHandle;
};

void runBlinker(FileRaiiGuard& chardev)
{
    using namespace std::chrono_literals;

    auto ledConfig = gpio_chardev_set_data{kLedGreen,1};
    while(true){
        chardev.passIoctl(GPIO_CHARDEV_SET_VALUE, ledConfig);
        std::this_thread::sleep_for(200ms);
        ledConfig.gpio_value = ~ledConfig.gpio_value;
        chardev.passIoctl(GPIO_CHARDEV_SET_VALUE, ledConfig);
    }
}
int main()
{
    constexpr std::string_view filePath = "/dev/gpio_chardev0";
    FileRaiiGuard gpioDevTest {filePath};
    runBlinker(gpioDevTest);
    return 0;
}