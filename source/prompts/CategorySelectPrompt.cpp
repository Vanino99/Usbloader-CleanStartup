/****************************************************************************
 * Copyright (C) 2011
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/
#include "CategorySelectPrompt.hpp"
#include "settings/CGameCategories.hpp"
#include "settings/CSettings.h"
#include "language/gettext.h"
#include "utils/StringTools.h"

CategorySelectPrompt::CategorySelectPrompt(struct discHdr * header)
    : CategoryPrompt(fmt("%s - %s", (char *) header->id, tr("Categories"))),
      gameHeader(header)
{
    categoryChanged.connect(this, &CategorySelectPrompt::OnCategoryChange);
    nextCategory.connect(this, &CategorySelectPrompt::OnNextCategory);
    previousCategory.connect(this, &CategorySelectPrompt::OnPreviousCategory);
    checkBoxClicked.connect(this, &CategorySelectPrompt::OnCheckboxClicked);
}

CategorySelectPrompt::~CategorySelectPrompt()
{
    int pos = GameCategories.CategoryList.pos()-1;
    GameCategories.Save();
    GameCategories.CategoryList.goToFirst();
    for(int i = 0; i < pos; ++i)
        GameCategories.CategoryList.goToNext();
}

void CategorySelectPrompt::OnNextCategory()
{
    GameCategories.CategoryList.goToNextCicle();
}

void CategorySelectPrompt::OnPreviousCategory()
{
    GameCategories.CategoryList.goToPreviousCicle();
}

void CategorySelectPrompt::OnCategoryChange(GuiText *catTxt, GuiText *positionTxt, GuiCheckbox *checkBox)
{
    catTxt->SetText(tr(GameCategories.CategoryList.getCurrentName().c_str()));
    positionTxt->SetTextf("%i / %i", GameCategories.CategoryList.pos()+1, GameCategories.CategoryList.size());
    checkBox->SetChecked(false);

    const vector<unsigned int> gameCat = GameCategories[gameHeader->id];
    for(u32 i = 0; i < gameCat.size(); ++i)
    {
        if(gameCat[i] == GameCategories.CategoryList.getCurrentID())
        {
            checkBox->SetChecked(true);
            break;
        }
    }
}

void CategorySelectPrompt::OnCheckboxClicked(GuiCheckbox *checkBox)
{
    if(GameCategories.CategoryList.getCurrentID() == 0)
    {
        checkBox->SetChecked(true);
        return;
    }

    const vector<unsigned int> gameCat = GameCategories[gameHeader->id];

    u32 i;
    for(i = 0; i < gameCat.size(); ++i)
    {
        if(gameCat[i] == GameCategories.CategoryList.getCurrentID())
        {
            if(!checkBox->IsChecked())
                GameCategories.RemoveCategory((const char *) gameHeader->id, gameCat[i]);
            break;
        }
    }

    if(i == gameCat.size() && checkBox->IsChecked())
    {
        GameCategories.SetCategory(gameHeader->id, GameCategories.CategoryList.getCurrentID());
    }
}
