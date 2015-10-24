Name:    privilege-checker
Summary: Privilege Management
Version: 0.0.4
Release: 3
Group:   System/Libraries
License: Apache-2.0
Source0: %{name}-%{version}.tar.gz
BuildRequires: cmake

%description
Privilege Management

%package -n privilege-checker-devel
summary: privilege-checker server
Group: Development/Libraries
Requires: privilege-checker = %{version}-%{release}
%description -n privilege-checker-devel
privilege-checker devel


%package  -n security-privilege-checker
Summary:  Privilege Checker API(Core)
Group:    TO_BE/FILLED_IN
BuildRequires:	pkgconfig(libsmack)
%if "%{?tizen_profile_name}" != "tv"
BuildRequires:	pkgconfig(privacy-manager-client)
%endif

%description -n security-privilege-checker
The Privilege Checker API provides functions to check given privilege is declared current application or not.

%package  -n security-privilege-checker-devel
Summary:  Privilege Checker API(Core)
Group:    TO_BE/FILLED_IN
BuildRequires: pkgconfig(libsmack)

%description -n security-privilege-checker-devel
The Privilege Checker API provides functions to check given privilege is declared current application or not.

%package -n capi-security-privilege-manager
Summary:    Privilege Manager API
Group:      TO_BE/FILLED_IN
License:    Apache-2.0
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  gettext-tools
BuildRequires:  pkgconfig(pkgmgr-info)
BuildRequires:  pkgconfig(cert-svc)
BuildRequires:  pkgconfig(sqlite3)
BuildRequires:  pkgconfig(glib-2.0)

%description -n capi-security-privilege-manager
The Privilege Manager API provides functions to get information about privilege information of installed packages.

%package  -n capi-security-privilege-manager-devel
Summary:  Privilege Manager API (Development)
Group:    TO_BE/FILLED_IN
Requires: capi-security-privilege-manager

%description -n capi-security-privilege-manager-devel
The Privilege Manager API provides functions to get information about privilege information of installed packages.(DEV)

%package  -n tc-privilege-checker
Summary:  tc-privilege-checker
Group:    TO_BE/FILLED_IN

%description -n tc-privilege-checker
tc-privilege-checker

%prep
%setup -q

%build

%if "%{?tizen_profile_name}" == "wearable"
    __PROFILE_TYPE="WEARABLE"
%else
%if "%{?tizen_profile_name}" == "mobile"
    __PROFILE_TYPE="MOBILE"
%else
%if "%{?tizen_profile_name}" == "tv"
    __PROFILE_TYPE="TV"
%endif
%endif
%endif

export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"

echo cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DDPL_LOG="ON" \
        -DPROFILE_TYPE="${__PROFILE_TYPE}"

cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DDPL_LOG="ON" \
        -DPROFILE_TYPE="${__PROFILE_TYPE}"

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.Apache-2.0 %{buildroot}/usr/share/license/privilege-checker
mkdir -p %{buildroot}/usr/share/privilege-manager
%if "%{?tizen_profile_name}" != "tv"
cp capi/res/dbspace/core_privilege_info.db %{buildroot}/usr/share/privilege-manager/.core_privilege_info.db
sqlite3 /%{buildroot}/usr/share/privilege-manager/.core_privilege_info.db "select * from privilege_info"
cp capi/res/dbspace/wrt_privilege_info.db %{buildroot}/usr/share/privilege-manager/.wrt_privilege_info.db
sqlite3 /%{buildroot}/usr/share/privilege-manager/.wrt_privilege_info.db "select * from privilege_info"
%else
# Add core privilege db for TV profile: After temporary support, it should be removed.
cp capi/res/dbspace/tv_core_privilege_info.db %{buildroot}/usr/share/privilege-manager/.core_privilege_info.db
sqlite3 /%{buildroot}/usr/share/privilege-manager/.core_privilege_info.db "select * from privilege_info"
cp capi/res/dbspace/tv_wrt_privilege_info.db %{buildroot}/usr/share/privilege-manager/.wrt_privilege_info.db
sqlite3 /%{buildroot}/usr/share/privilege-manager/.wrt_privilege_info.db "select * from privilege_info"
%endif
%make_install

%files -n privilege-checker
/usr/share/license/privilege-checker
%manifest packaging/privilege-checker.manifest

%files -n security-privilege-checker
%{_libdir}/libsecurity-privilege-checker.so*
%manifest packaging/security-privilege-checker.manifest

%files -n security-privilege-checker-devel
%{_includedir}/privilege_checker.h
%{_libdir}/pkgconfig/security-privilege-checker.pc
%{_libdir}/libsecurity-privilege-checker.so

%files -n capi-security-privilege-manager
%{_libdir}/libcapi-security-privilege-manager.so*
/usr/share/locale/*
#%if "%{?tizen_profile_name}" != "tv"
/usr/share/privilege-manager/.core_privilege_info.db
#%endif
/usr/share/privilege-manager/.wrt_privilege_info.db
%manifest packaging/capi-security-privilege-manager.manifest

%files -n capi-security-privilege-manager-devel
%{_includedir}/privilegemgr/*.h
%{_libdir}/pkgconfig/capi-security-privilege-manager.pc

%files -n tc-privilege-checker
%manifest test/org.tizen.test-privilege-checker.manifest
/usr/apps/org.tizen.test-privilege-checker/bin/tc-privilege-checker
/usr/share/packages/org.tizen.test-privilege-checker.xml
%{_bindir}/tc-privilege-db-manager
%{_bindir}/tc-privilege-manager
%{_bindir}/tc-privilege-info

%clean
rm -rf %{buildroot}

