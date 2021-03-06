#include <ntddk.h>
#include <wdf.h>

#include "Trace.h"
#include "Device.h"
#include "Device.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, CreateDevice)
#pragma alloc_text (PAGE, DeleteDevice)
#endif

NTSTATUS
CreateDevice(
    _In_ WDFDRIVER Driver,
    _In_ WDFDEVICE* Device
)
{
    PWDFDEVICE_INIT deviceInit;
    NTSTATUS status;

    // Allocate a device initialization structure
    deviceInit = WdfControlDeviceInitAllocate(
        Driver,
        &SDDL_DEVOBJ_KERNEL_ONLY
    );

    if (deviceInit == NULL)
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "%!FUNC! WdfControlDeviceInitAllocate failed");
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    // Set the device characteristics
    WdfDeviceInitSetCharacteristics(
        deviceInit,
        FILE_DEVICE_SECURE_OPEN | FILE_AUTOGENERATED_DEVICE_NAME,
        FALSE
    );

    // Create a framework device object
    status = WdfDeviceCreate(
        &deviceInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        Device
    );

    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "%!FUNC! WdfDeviceCreate failed %!STATUS!", status);
        return status;
    }

    // Initialization of the framework device object is complete
    WdfControlFinishInitializing(*Device);

    return status;
}

_Use_decl_annotations_
VOID
DeleteDevice(
    _In_ WDFDEVICE Device
)
{
    WdfObjectDelete(Device);
}
