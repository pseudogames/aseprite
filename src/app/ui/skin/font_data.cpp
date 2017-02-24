// Aseprite
// Copyright (C) 2001-2017  David Capello
//
// This program is distributed under the terms of
// the End-User License Agreement for Aseprite.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "app/ui/skin/font_data.h"

#include "she/font.h"
#include "she/system.h"

namespace app {
namespace skin {

FontData::FontData(she::FontType type)
  : m_type(type)
  , m_antialias(false)
  , m_fallback(nullptr)
  , m_fallbackSize(0)
{
}

FontData::~FontData()
{
  // Destroy all fonts
  for (auto& it : m_fonts)
    it.second->dispose();
}

she::Font* FontData::getFont(int size)
{
  if (m_type == she::FontType::kSpriteSheet)
    size = 0;                   // Same size always

  auto it = m_fonts.find(size);
  if (it != m_fonts.end())
    return it->second;

  she::Font* font = nullptr;

  switch (m_type) {
    case she::FontType::kSpriteSheet:
      font = she::instance()->loadSpriteSheetFont(m_filename.c_str());
      break;
    case she::FontType::kTrueType:
      font = she::instance()->loadTrueTypeFont(m_filename.c_str(), size);
      if (font)
        font->setAntialias(m_antialias);
      break;
  }

  if (font && m_fallback)
    font->setFallback(m_fallback->getFont(m_fallbackSize));

  return m_fonts[size] = font;
}

} // namespace skin
} // namespace app
