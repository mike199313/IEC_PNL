/*
 * INTEL CONFIDENTIAL
 *
 * Copyright 2020-2022 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with
 * no express or implied warranties, other than those that are expressly
 * stated in the License.
 */

#pragma once

#include "domain_power.hpp"
#include "policies/policy_factory.hpp"
#include "statistics/global_accumulator.hpp"
#include "statistics/statistic.hpp"
#include "utility/devices_configuration.hpp"

namespace nodemanager
{

static const std::shared_ptr<std::vector<TriggerType>>
    kTriggersInDomainCpuSubsystem =
        std::make_shared<std::vector<TriggerType>>(std::vector<TriggerType>{
            TriggerType::always, TriggerType::inletTemperature,
            TriggerType::gpio, TriggerType::cpuUtilization,
            TriggerType::hostReset, TriggerType::smbalertInterrupt});

class DomainCpuSubsystem : public DomainPower
{
  public:
    DomainCpuSubsystem() = delete;
    DomainCpuSubsystem(const DomainCpuSubsystem&) = delete;
    DomainCpuSubsystem& operator=(const DomainCpuSubsystem&) = delete;
    DomainCpuSubsystem(DomainCpuSubsystem&&) = delete;
    DomainCpuSubsystem& operator=(DomainCpuSubsystem&&) = delete;

    DomainCpuSubsystem(
        const std::shared_ptr<sdbusplus::asio::connection>& busArg,
        const std::shared_ptr<sdbusplus::asio::object_server>& objServerArg,
        std::string const& objectPathArg,
        const std::shared_ptr<DevicesManagerIf>& devicesManagerArg,
        const std::shared_ptr<GpioProviderIf>& gpioProviderArg,
        const std::shared_ptr<TriggersManagerIf>& triggersManagerArg,
        const std::shared_ptr<BudgetingIf>& budgetingArg,
        const std::shared_ptr<PolicyFactoryIf>& policyFactoryArg,
        const std::shared_ptr<CapabilitiesFactoryIf>& capabilitiesFactory,
        const DbusState dbusState) :
        DomainPower(busArg, objServerArg, objectPathArg, devicesManagerArg,
                    gpioProviderArg, triggersManagerArg, budgetingArg, ID,
                    ReadingType::cpuPackagePower,
                    ReadingType::cpuPackagePowerCapabilitiesMin,
                    ReadingType::cpuPackagePowerCapabilitiesMax,
                    ReadingType::cpuPackagePowerLimit, ReadingType::cpuEnergy,
                    policyFactoryArg, capabilitiesFactory, kMaxCpuNumber,
                    kTriggersInDomainCpuSubsystem, kPolicyMinCorrectionTimeInMs,
                    dbusState)
    {
        registerAvailableComponents<kMaxCpuNumber>(ReadingType::cpuPresence);
    }

    virtual ~DomainCpuSubsystem() = default;

    void createDefaultPolicies() override
    {
        createDmtfPolicies("_Processor", "_Processors");
    }

    static constexpr DomainId ID = DomainId::CpuSubsystem;
};

} // namespace nodemanager
