/*
   This file is part of the clazy static checker.

  Copyright (C) 2016 Sergio Martins <smartins@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "qfileinfo-exists.h"
#include "Utils.h"
#include "checkmanager.h"
#include "StringUtils.h"

#include <clang/AST/AST.h>
#include <clang/Lex/Lexer.h>

using namespace clang;
using namespace std;


QFileInfoExists::QFileInfoExists(const std::string &name, const clang::CompilerInstance &ci)
    : CheckBase(name, ci)
{
}

void QFileInfoExists::VisitStmt(clang::Stmt *stmt)
{
    auto existsCall = dyn_cast<CXXMemberCallExpr>(stmt);
    std::string methodName = StringUtils::qualifiedMethodName(existsCall);
    if (methodName != "QFileInfo::exists")
        return;

    CXXConstructExpr* ctorExpr = HierarchyUtils::getFirstChildOfType<CXXConstructExpr>(existsCall);
    if (!ctorExpr || StringUtils::simpleArgTypeName(ctorExpr->getConstructor(), 0, lo()) != "QString")
        return;

    emitWarning(stmt->getLocStart(), "Use the static QFileInfo::exists() instead");
}


REGISTER_CHECK_WITH_FLAGS("qfileinfo-exists", QFileInfoExists, CheckLevel0)
