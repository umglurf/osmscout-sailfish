/*
  This source is part of the libosmscout library
  Copyright (C) 2009  Tim Teulings

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

#include <osmscout/StyleConfig.h>

#include <set>

#include <iostream>

namespace osmscout {

  LineStyle::LineStyle()
   : lineColor(1.0,0.0,0.0,0.0),
     alternateColor(1,0.0,0.0,0.0),
     outlineColor(1,0.0,0.0,0.0),
     gapColor(1,0.0,0.0,0.0),
     displayWidth(0),
     width(0),
     fixedWidth(false),
     outline(0)
  {
    // no code
  }

  LineStyle& LineStyle::SetLineColor(const Color& color)
  {
    this->lineColor=color;
    this->alternateColor=color;

    return *this;
  }

  LineStyle& LineStyle::SetAlternateColor(const Color& color)
  {
    this->alternateColor=color;

    return *this;
  }

  LineStyle& LineStyle::SetOutlineColor(const Color& color)
  {
    outlineColor=color;

    return *this;
  }

  LineStyle& LineStyle::SetGapColor(const Color& color)
  {
    gapColor=color;

    return *this;
  }

  LineStyle& LineStyle::SetDisplayWidth(double value)
  {
    displayWidth=value;

    return *this;
  }

  LineStyle& LineStyle::SetWidth(double value)
  {
    width=value;

    return *this;
  }

  LineStyle& LineStyle::SetFixedWidth(bool fixedWidth)
  {
    this->fixedWidth=fixedWidth;

    return *this;
  }

  LineStyle& LineStyle::SetOutline(double value)
  {
    outline=value;

    return *this;
  }

  LineStyle& LineStyle::SetDashes(const std::vector<double> dashes)
  {
    dash=dashes;

    return *this;
  }


  FillStyle::FillStyle()
   : fillColor(1.0,0.0,0.0,0.0),
     patternId(0),
     patternMinMag(magWorld),
     borderColor(1.0,0.0,0.0,0.0),
     borderWidth(0.0)
  {
    // no code
  }

  FillStyle& FillStyle::SetFillColor(const Color& color)
  {
    fillColor=color;

    return *this;
  }

  void FillStyle::SetPatternId(size_t id) const
  {
    patternId=id;
  }

  FillStyle& FillStyle::SetPattern(const std::string& pattern)
  {
    this->pattern=pattern;

    return *this;
  }

  FillStyle& FillStyle::SetPatternMinMag(Mag mag)
  {
    patternMinMag=mag;

    return *this;
  }

  FillStyle& FillStyle::SetBorderColor(const Color& color)
  {
    borderColor=color;

    return *this;
  }

  FillStyle& FillStyle::SetBorderWidth(double value)
  {
    borderWidth=value;

    return *this;
  }

  FillStyle& FillStyle::SetBorderDashes(const std::vector<double> dashes)
  {
    borderDash=dashes;

    return *this;
  }

  LabelStyle::LabelStyle()
   : style(none),
     priority(0),
     scaleAndFadeMag((Mag)1000000),
     size(1),
     textColor(0,0,0),
     bgColor(1,1,1),
     borderColor(0,0,0)
  {
    // no code
  }

  LabelStyle& LabelStyle::SetStyle(Style style)
  {
    this->style=style;

    return *this;
  }

  LabelStyle& LabelStyle::SetPriority(uint8_t priority)
  {
    this->priority=priority;

    return *this;
  }

  LabelStyle& LabelStyle::SetScaleAndFadeMag(Mag mag)
  {
    this->scaleAndFadeMag=mag;

    return *this;
  }

  LabelStyle& LabelStyle::SetSize(double size)
  {
    this->size=size;

    return *this;
  }

  LabelStyle& LabelStyle::SetTextColor(const Color& color)
  {
    this->textColor=color;

    return *this;
  }

  LabelStyle& LabelStyle::SetBgColor(const Color& color)
  {
    this->bgColor=color;

    return *this;
  }

  LabelStyle& LabelStyle::SetBorderColor(const Color& color)
  {
    this->borderColor=color;

    return *this;
  }

  SymbolStyle::SymbolStyle()
   : style(none),
     size(8),
     fillColor(1,0,0)
  {
    // no code
  }

  SymbolStyle& SymbolStyle::SetStyle(Style style)
  {
    this->style=style;

    return *this;
  }

  SymbolStyle& SymbolStyle::SetSize(double size)
  {
    this->size=size;

    return *this;
  }

  SymbolStyle& SymbolStyle::SetFillColor(const Color& color)
  {
    fillColor=color;

    return *this;
  }

  IconStyle::IconStyle()
   : id(0)
  {
    // no code
  }

  IconStyle& IconStyle::SetId(size_t id)
  {
    this->id=id;

    return *this;
  }

  IconStyle& IconStyle::SetIconName(const std::string& iconName)
  {
    this->iconName=iconName;

    return *this;
  }

  StyleFilter::StyleFilter()
  : minLevel(0),
    maxLevel(std::numeric_limits<size_t>::max())
  {
    // no code
  }

  StyleFilter& StyleFilter::SetTypes(const TypeSet& types)
  {
    this->types=types;

    return *this;
  }

  StyleFilter& StyleFilter::SetMinLevel(size_t level)
  {
    minLevel=level;

    return *this;
  }

  StyleFilter& StyleFilter::SetMaxLevel(size_t level)
  {
    maxLevel=level;

    return *this;
  }

  bool StyleFilter::HasMaxLevel() const
  {
    return maxLevel!=std::numeric_limits<size_t>::max();
  }

  StyleConfig::StyleConfig(TypeConfig* typeConfig)
   : typeConfig(typeConfig)
  {
    nodeSymbolStyles.resize(typeConfig->GetMaxTypeId()+1);
    nodeRefLabelStyles.resize(typeConfig->GetMaxTypeId()+1);
    nodeLabelStyles.resize(typeConfig->GetMaxTypeId()+1);
    nodeIconStyles.resize(typeConfig->GetMaxTypeId()+1);

    wayPrio.resize(typeConfig->GetMaxTypeId()+1,std::numeric_limits<size_t>::max());
    wayLineStyles.resize(typeConfig->GetMaxTypeId()+1);
    wayRefLabelStyles.resize(typeConfig->GetMaxTypeId()+1);
    wayNameLabelStyles.resize(typeConfig->GetMaxTypeId()+1);

    areaFillStyles.resize(typeConfig->GetMaxTypeId()+1);
    areaSymbolStyles.resize(typeConfig->GetMaxTypeId()+1);
    areaLabelStyles.resize(typeConfig->GetMaxTypeId()+1);
    areaIconStyles.resize(typeConfig->GetMaxTypeId()+1);

    std::cout << "Size long: " << sizeof(long) << std::endl;
  }

  StyleConfig::~StyleConfig()
  {
    // no code
  }

  void StyleConfig::GetAllNodeTypes(std::list<TypeId>& types)
  {
    for (TypeId t=0; t<=typeConfig->GetMaxTypeId(); t++) {
      if (typeConfig->GetTypeInfo(t).CanBeNode()) {
        types.push_back(t);
      }
    }
  }

  void StyleConfig::GetAllWayTypes(std::list<TypeId>& types)
  {
    for (TypeId t=0; t<=typeConfig->GetMaxTypeId(); t++) {
      if (typeConfig->GetTypeInfo(t).CanBeWay()) {
        types.push_back(t);
      }
    }
  }

  void StyleConfig::GetAllAreaTypes(std::list<TypeId>& types)
  {
    for (TypeId t=0; t<=typeConfig->GetMaxTypeId(); t++) {
      if (typeConfig->GetTypeInfo(t).CanBeArea()) {
        types.push_back(t);
      }
    }
  }

  template<typename S>
  void CleanupStyles(std::vector<std::vector<Ref<S> > >& styles)
  {
    for (TypeId type=0;type<styles.size(); type++) {
      for (size_t level=0; level<styles[type].size(); level++) {
        if (styles[type][level].Valid() &&
            !styles[type][level]->IsVisible()) {
          styles[type][level]=NULL;
        }
      }
    }
  }

  template<typename S>
  bool HasStyle(const std::vector<Ref<S> >& styles, size_t level)
  {
    size_t size=styles.size();

    if (size==0) {
      return false;
    }

    if (level<size) {
      return styles[level].Valid();
    }
    else {
      return styles[size-1].Valid();
    }
  }

  void StyleConfig::PostprocessNodes()
  {
    CleanupStyles(nodeLabelStyles);
    CleanupStyles(nodeRefLabelStyles);
    CleanupStyles(nodeSymbolStyles);
    CleanupStyles(nodeIconStyles);

    size_t maxLevel=0;
    for (TypeId type=0; type<=typeConfig->GetMaxTypeId(); type++) {
      if (!typeConfig->GetTypeInfo(type).CanBeNode()) {
        continue;
      }

      maxLevel=std::max(maxLevel,nodeLabelStyles[type].size());
      maxLevel=std::max(maxLevel,nodeRefLabelStyles[type].size());
      maxLevel=std::max(maxLevel,nodeSymbolStyles[type].size());
      maxLevel=std::max(maxLevel,nodeIconStyles[type].size());
    }

    nodeTypeSets.reserve(maxLevel);

    for (size_t type=0; type<maxLevel; type++) {
      nodeTypeSets.push_back(TypeSet(*typeConfig));
    }

    for (size_t level=0;
        level<maxLevel;
        ++level) {
      for (TypeId type=0; type<=typeConfig->GetMaxTypeId(); type++) {
        if (!typeConfig->GetTypeInfo(type).CanBeNode()) {
          continue;
        }

        if (HasStyle(nodeLabelStyles[type],level)) {
          nodeTypeSets[level].SetType(type);
        }
        else if (HasStyle(nodeRefLabelStyles[type],level)) {
          nodeTypeSets[level].SetType(type);
        }
        else if (HasStyle(nodeSymbolStyles[type],level)) {
          nodeTypeSets[level].SetType(type);
        }
        else if (HasStyle(nodeIconStyles[type],level)) {
          nodeTypeSets[level].SetType(type);
        }
      }
    }
  }

  void StyleConfig::PostprocessWays()
  {
    CleanupStyles(wayLineStyles);
    CleanupStyles(wayNameLabelStyles);
    CleanupStyles(wayRefLabelStyles);

    size_t maxLevel=0;
    for (TypeId type=0; type<=typeConfig->GetMaxTypeId(); type++) {
      if (!typeConfig->GetTypeInfo(type).CanBeWay()) {
        continue;
      }

      maxLevel=std::max(maxLevel,wayLineStyles[type].size());
      maxLevel=std::max(maxLevel,wayNameLabelStyles[type].size());
      maxLevel=std::max(maxLevel,wayRefLabelStyles[type].size());
    }

    wayTypeSets.resize(maxLevel);

    std::set<size_t> prios;

    for (TypeId type=0; type<wayPrio.size(); type++) {
      prios.insert(wayPrio[type]);
    }

    for (size_t level=0;
        level<maxLevel;
        ++level) {
      for (std::set<size_t>::const_iterator prio=prios.begin();
          prio!=prios.end();
          ++prio) {
        TypeSet typeSet(*typeConfig);

        for (TypeId type=0; type<wayPrio.size(); type++) {
          if (!typeConfig->GetTypeInfo(type).CanBeWay()) {
            continue;
          }

          if (wayPrio[type]==*prio) {
            if (HasStyle(wayLineStyles[type],level)) {
              typeSet.SetType(type);
            }
            else if (HasStyle(wayRefLabelStyles[type],level)) {
              typeSet.SetType(type);
            }
            else if (HasStyle(wayNameLabelStyles[type],level)) {
              typeSet.SetType(type);
            }
          }
        }

        // TODO: Is type set not empty?
        wayTypeSets[level].push_back(typeSet);
      }
    }
  }

  void StyleConfig::PostprocessAreas()
  {
    CleanupStyles(areaFillStyles);
    CleanupStyles(areaSymbolStyles);
    CleanupStyles(areaLabelStyles);
    CleanupStyles(areaIconStyles);

    size_t maxLevel=0;
    for (TypeId type=0; type<=typeConfig->GetMaxTypeId(); type++) {
      if (!typeConfig->GetTypeInfo(type).CanBeArea()) {
        continue;
      }

      maxLevel=std::max(maxLevel,areaFillStyles[type].size());
      maxLevel=std::max(maxLevel,areaSymbolStyles[type].size());
      maxLevel=std::max(maxLevel,areaLabelStyles[type].size());
      maxLevel=std::max(maxLevel,areaIconStyles[type].size());
    }

    areaTypeSets.reserve(maxLevel);

    for (size_t type=0; type<maxLevel; type++) {
      areaTypeSets.push_back(TypeSet(*typeConfig));
    }

    for (size_t level=0;
        level<maxLevel;
        ++level) {
      for (TypeId type=0; type<areaFillStyles.size(); type++) {
        if (!typeConfig->GetTypeInfo(type).CanBeArea()) {
          continue;
        }

        if (HasStyle(areaFillStyles[type],level)) {
          areaTypeSets[level].SetType(type);
        }
        else if (HasStyle(areaSymbolStyles[type],level)) {
          areaTypeSets[level].SetType(type);
        }
        else if (HasStyle(areaLabelStyles[type],level)) {
          areaTypeSets[level].SetType(type);
        }
        else if (HasStyle(areaIconStyles[type],level)) {
          areaTypeSets[level].SetType(type);
        }
      }
    }
  }

  void StyleConfig::Postprocess()
  {
    PostprocessNodes();
    PostprocessWays();
    PostprocessAreas();
  }

  TypeConfig* StyleConfig::GetTypeConfig() const
  {
    return typeConfig;
  }

  StyleConfig& StyleConfig::SetWayPrio(TypeId type, size_t prio)
  {
    wayPrio[type]=prio;

    return *this;
  }

  template<typename S>
  void GetMatchingStyles(const TypeConfig& typeConfig,
                         const StyleFilter& filter,
                         std::vector<std::vector<Ref<S> > >& allStyles,
                         std::list<Ref<S> >& matchingStyles)
  {
    // Create style, if not available yet
    for (TypeId type=0;
        type<=typeConfig.GetMaxTypeId();
        type++) {
      if (!filter.HasType(type)) {
        continue;
      }

      size_t maxLevel;

      if (filter.HasMaxLevel()) {
        maxLevel=filter.GetMaxLevel()+1;
      }
      else {
        maxLevel=filter.GetMinLevel();
      }

      assert(type<allStyles.size());

      if (allStyles[type].size()<maxLevel+1) {
        size_t oldSize=allStyles[type].size();
        Ref<S> originalValue;

        if (oldSize>0) {
          originalValue=allStyles[type][oldSize-1];
        }
        else {
          originalValue=new S();
        }

        allStyles[type].resize(maxLevel+1);

        for (size_t i=oldSize; i<allStyles[type].size(); i++) {
          allStyles[type][i]=new S(*originalValue);
        }

        if (filter.HasMaxLevel()) {
          allStyles[type][filter.GetMaxLevel()+1]=new S();
        }
      }
    }

    for (TypeId type=0;
        type<=typeConfig.GetMaxTypeId();
        type++) {
      if (!filter.HasType(type)) {
        continue;
      }

      size_t minIndex=filter.GetMinLevel();
      size_t maxIndex;

      if (filter.HasMaxLevel()) {
        maxIndex=filter.GetMaxLevel();
      }
      else {
        maxIndex=allStyles[type].size()-1;
      }

      for (size_t i=minIndex; i<=maxIndex; i++) {
        assert(type<allStyles.size());
        assert(i<allStyles[type].size());
        assert(allStyles[type][i].Valid());
        matchingStyles.push_back(allStyles[type][i]);
      }
    }
  }

  void StyleConfig::GetNodeSymbolStyles(const StyleFilter& filter,
                                        std::list<SymbolStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      nodeSymbolStyles,
                      styles);
  }

  void StyleConfig::GetNodeRefLabelStyles(const StyleFilter& filter,
                                          std::list<LabelStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      nodeRefLabelStyles,
                      styles);
  }

  void StyleConfig::GetNodeNameLabelStyles(const StyleFilter& filter,
                                           std::list<LabelStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      nodeLabelStyles,
                      styles);
  }

  void StyleConfig::GetNodeIconStyles(const StyleFilter& filter,
                                      std::list<IconStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      nodeIconStyles,
                      styles);
  }

  void StyleConfig::GetWayLineStyles(const StyleFilter& filter,
                                     std::list<LineStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      wayLineStyles,
                      styles);
  }

  void StyleConfig::GetWayRefLabelStyles(const StyleFilter& filter,
                                         std::list<LabelStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      wayRefLabelStyles,
                      styles);
  }

  void StyleConfig::GetWayNameLabelStyles(const StyleFilter& filter, std::list<LabelStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      wayNameLabelStyles,
                      styles);
  }

  void StyleConfig::GetAreaFillStyles(const StyleFilter& filter, std::list<FillStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      areaFillStyles,
                      styles);
  }

  void StyleConfig::GetAreaLabelStyles(const StyleFilter& filter, std::list<LabelStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      areaLabelStyles,
                      styles);

  }

  void StyleConfig::GetAreaSymbolStyles(const StyleFilter& filter, std::list<SymbolStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      areaSymbolStyles,
                      styles);

  }

  void StyleConfig::GetAreaIconStyles(const StyleFilter& filter, std::list<IconStyleRef>& styles)
  {
    styles.clear();

    GetMatchingStyles(*typeConfig,
                      filter,
                      areaIconStyles,
                      styles);

  }

  void StyleConfig::GetNodeTypesWithMaxMag(double maxMag,
                                           TypeSet& types) const
  {
    if (!nodeTypeSets.empty()) {
      types=nodeTypeSets[std::min(MagToLevel(maxMag),nodeTypeSets.size()-1)];
    }
  }

  void StyleConfig::GetWayTypesByPrioWithMaxMag(double maxMag,
                                             std::vector<TypeSet>& types) const
  {
    if (!wayTypeSets.empty()) {
      types=wayTypeSets[std::min(MagToLevel(maxMag),wayTypeSets.size()-1)];
    }
  }

  void StyleConfig::GetAreaTypesWithMaxMag(double maxMag,
                                           TypeSet& types) const
  {
    if (!areaTypeSets.empty()) {
      types=areaTypeSets[std::min(MagToLevel(maxMag),areaTypeSets.size()-1)];
    }
  }

  template<typename S>
  S* GetStyle(const std::vector<std::vector<Ref<S> > >& styles,
              TypeId type,
              size_t level)
  {
    if (type>=styles.size()) {
      return NULL;
    }

    size_t size=styles[type].size();

    if (level<size) {
      return styles[type][level];
    }
    else if (size>0) {
      return styles[type][size-1];
    }
    else {
      return NULL;
    }
  }

  SymbolStyle* StyleConfig::GetNodeSymbolStyle(TypeId type, size_t level) const
  {
    return GetStyle(nodeSymbolStyles,type,level);
  }

  IconStyle* StyleConfig::GetNodeIconStyle(TypeId type,
                                           size_t level) const
  {
    return GetStyle(nodeIconStyles,type,level);
  }

  LabelStyle* StyleConfig::GetNodeRefLabelStyle(TypeId type, size_t level) const
  {
    return GetStyle(nodeRefLabelStyles,type,level);
  }

  LabelStyle* StyleConfig::GetNodeLabelStyle(TypeId type, size_t level) const
  {
    return GetStyle(nodeLabelStyles,type,level);
  }

  LineStyle* StyleConfig::GetWayLineStyle(TypeId type, size_t level) const
  {
    return GetStyle(wayLineStyles,type,level);
  }

  LabelStyle* StyleConfig::GetWayRefLabelStyle(TypeId type, size_t level) const
  {
    return GetStyle(wayRefLabelStyles,type,level);
  }

  LabelStyle* StyleConfig::GetWayNameLabelStyle(TypeId type, size_t level) const
  {
    return GetStyle(wayNameLabelStyles,type,level);
  }

  FillStyle* StyleConfig::GetAreaFillStyle(TypeId type, size_t level) const
  {
    return GetStyle(areaFillStyles,type,level);
  }

  LabelStyle* StyleConfig::GetAreaLabelStyle(TypeId type, size_t level) const
  {
    return GetStyle(areaLabelStyles,type,level);
  }

  SymbolStyle* StyleConfig::GetAreaSymbolStyle(TypeId type, size_t level) const
  {
    return GetStyle(areaSymbolStyles,type,level);
  }

  IconStyle* StyleConfig::GetAreaIconStyle(TypeId type, size_t level) const
  {
    return GetStyle(areaIconStyles,type,level);
  }
}
